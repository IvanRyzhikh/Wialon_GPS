#include <array>

#include <fcntl.h>
#include <unistd.h>
#include <etl/string.h>
#include <etl/to_string.h>
#include <boost/assert.hpp>
#include <errcode/errcode.h>

#include "law/File/File.h"
#include "law/GPIO/SysfsGPIO.h"

namespace law::gpio {
    static constexpr std::uint8_t kQuantityOfPinsInPort = 32;
    static constexpr std::string_view kCommonPinPath = "/sys/class/gpio/gpio";

    static std::uint16_t gpioPinPortToSysfsPinNumber(Port port, std::uint8_t pinNumber) noexcept {
        return (static_cast<std::uint8_t>(port) * kQuantityOfPinsInPort + pinNumber - kQuantityOfPinsInPort);
    }

    static bool isFileExported(std::string_view filePath) noexcept {
        return (access(filePath.data(), F_OK) == 0);
    }

    static Status exportPin(std::uint16_t sysfsPinNumber) noexcept {
        using ExportFilePath = etl::string<sizeof("/sys/class/gpio/export") - 1>;

        static constexpr std::string_view kGenericPart = "/sys/class/gpio/export";
        ExportFilePath exportFilePath(kGenericPart.data(), kGenericPart.size());

        using ExportPinString = etl::string<sizeof("XXX\n") - 1>;
        ExportPinString exportPinString;
        etl::to_string(sysfsPinNumber, exportPinString);
        exportPinString += '\n';

        file::ExtBufferFile exportFile;
        return exportFile.open({exportFilePath.data(), exportFilePath.size()}, law::file::WriteOnly())
                .and_then(
                    [&exportFile, &exportPinString] noexcept {
                        return exportFile.write({exportPinString.data(), exportPinString.size()});
                    }
                )
                .and_then(
                    [](std::size_t) noexcept {
                        return Status();
                    }
                );
    }

    SysfsGPIO::SysfsGPIO(Port gpioPort, std::uint8_t pinNumber) {
        if (!setPin(gpioPort, pinNumber)) {
            throwErrno();
        }
    }

    SysfsGPIO::SysfsGPIO(std::uint16_t sysfsPinNumber) {
        if (!setPin(sysfsPinNumber)) {
            throwErrno();
        }
    }

    Status SysfsGPIO::setPinMode(PinMode mode) noexcept {
        using DirectionFilePath = etl::string<34>;

        DirectionFilePath directionFilePath(kCommonPinPath.data(), kCommonPinPath.size());
        etl::to_string(sysfsPinNum, directionFilePath, true);

        static constexpr std::string_view kGenericPart = "/direction";
        directionFilePath.append(kGenericPart.data(), kGenericPart.size());

        file::ExtBufferFile directionFile;
        return directionFile.open({directionFilePath.data(), directionFilePath.size()}, file::WriteOnly())
                .and_then(
                    [mode, &directionFile] noexcept {
                        if (mode == PinMode::INPUT) {
                            static constexpr std::string_view kInValue = "in\n";
                            return directionFile.write(kInValue);
                        }

                        static constexpr std::string_view kOutValue = "out\n";
                        return directionFile.write(kOutValue);
                    }
                )
                .and_then(
                    [](std::size_t) noexcept {
                        return Status();
                    }
                );


        return {};
    }

    Status SysfsGPIO::setPin(Port port, std::uint8_t pinNumber) noexcept {
        if (pinNumber > kQuantityOfPinsInPort) {
            return fromLawError(RetCode::INVALID_ARGUMENT);
        }

        return setPin(gpioPinPortToSysfsPinNumber(port, pinNumber));
    }

    Status SysfsGPIO::setPin(std::uint16_t sysfsPinNumber) noexcept {
        using ValueFilePath = etl::string<29>;

        ValueFilePath valueFilePath(kCommonPinPath.data(), kCommonPinPath.size());
        sysfsPinNum = sysfsPinNumber;
        etl::to_string(sysfsPinNum, valueFilePath, true);

        static constexpr std::string_view kGenericPart = "/value";
        valueFilePath.append(kGenericPart.data(), kGenericPart.size());

        if (!isFileExported({valueFilePath.data(), valueFilePath.size()})) {
            EXPECTED_CHECK(exportPin(sysfsPinNum));
        }

        if (operator bool()) {
            static_cast<void>(close());
        }

        auto fd = open(valueFilePath.data(), O_RDWR);
        if (fd == -1) [[unlikely]] {
            return fromErrno();
        }

        setNativeHandler(&fd);
        return {};
    }

    Status SysfsGPIO::setEdge(Edge edge) noexcept {
        using EdgeFilePath = etl::string<28>;

        EdgeFilePath edgeFilePath(kCommonPinPath.data(), kCommonPinPath.size());
        etl::to_string(sysfsPinNum, edgeFilePath, true);

        static constexpr std::string_view kGenericPart = "/edge";
        edgeFilePath.append(kGenericPart.data(), kGenericPart.size());

        file::ExtBufferFile directionFile;
        return directionFile.open({edgeFilePath.data(), edgeFilePath.size()}, file::WriteOnly())
                .and_then(
                    [edge, &directionFile] noexcept {
                        static constexpr
                                std::array<std::string_view, std::uint8_t(Edge::MAX_VALUE)> kEnumStringValues = {
                                    "none\n", "rising\n", "falling\n", "both\n"
                                };
                        BOOST_ASSERT(edge < Edge::MAX_VALUE);

                        return directionFile.write(kEnumStringValues[std::size_t(edge)]);
                    }
                )
                .and_then(
                    [](std::size_t) noexcept {
                        return Status();
                    }
                );
    }

    Status SysfsGPIO::set() noexcept {
        return setState(true);
    }

    Status SysfsGPIO::reset() noexcept {
        return setState(false);
    }

    Status SysfsGPIO::setState(bool status) noexcept {
        return resetValueFilePos()
                .and_then(
                    [this, status] noexcept {
                        const char value = '0' + status;
                        if (write(getNativeHandler(), &value, sizeof(char)) == -1) [[unlikely]] {
                            return fromErrno();
                        }

                        return Status();
                    }
                );
    }

    BoolRet SysfsGPIO::get() noexcept {
        return resetValueFilePos()
                .and_then(
                    [this]
                noexcept -> BoolRet {
                        char value;
                        if (read(getNativeHandler(), &value, sizeof(char)) == -1) [[unlikely]] {
                            return fromErrno<Bool>();
                        }

                        return (value - '0');
                    }
                );
    }

    Status SysfsGPIO::resetValueFilePos() noexcept {
        return lseek(getNativeHandler(), 0, SEEK_SET) == -1 ? fromErrno() : Status();
    }
}
