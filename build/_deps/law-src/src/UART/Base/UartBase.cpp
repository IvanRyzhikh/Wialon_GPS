#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <asm/ioctls.h>

#include "law/Utils.h"
#include "law/RetCode/RetCode.h"
#include "law/UART/Base/UartBase.h"

namespace law::uart::detail {
    UartBase::UartBase(std::string_view tty, const UartConfiguration &uartConf) {
        if (!attach(tty, uartConf)) {
            throw std::system_error(errno, std::system_category());
        }
    }

    UartBase::~UartBase() noexcept {
        if (operator bool()) {
            flock(this->getNativeHandler(), LOCK_UN);
        }
    }

    Status UartBase::reattach(std::string_view tty, const UartConfiguration &uartConf) {
        close();
        return attach(tty, uartConf);
    }

    Status UartBase::attach(std::string_view tty, const UartConfiguration &uartConf) {
        if (operator bool()) {
            return {};
        }

        auto fd = ::open(tty.data(), O_RDWR | O_NOCTTY);
        if (fd == -1) {
            return fromErrno();
        }

        setNativeHandler(&fd);
        auto rc = lockTty();
        if (!rc) {
            return rc;
        }

        rc = applyUartConfiguration(uartConf);
        if (!rc) {
            return rc;
        }

        uartConfiguration = uartConf;
        return {};
    }

    Status UartBase::applyUartConfiguration(const UartConfiguration &uartConf) noexcept {
        auto rc = applyOption(TCSETS2, &uartConf.getConfiguration());
        if (!rc) {
            return rc;
        }

        uartConfiguration = uartConf;
        return {};
    }

    const uart::UartConfiguration &UartBase::getUartConfiguration() const noexcept {
        return uartConfiguration;
    }

    void UartBase::setDefaultReadCount(std::uint8_t readCount) noexcept {
        currentDefaultReadCount = readCount;
    }

    std::uint8_t UartBase::getDefaultReadCount() const noexcept {
        return currentDefaultReadCount;
    }

    WriteCountRet UartBase::write(std::string_view data) noexcept {
        auto writtenBytesCount = ::write(getNativeHandler(), data.data(), data.size());
        if (writtenBytesCount == -1) {
            return fromErrno<WriteCount>();
        }

        return writtenBytesCount;
    }

    WriteCountRet UartBase::write(const void *buf, std::size_t size) noexcept {
        return write({reinterpret_cast<const char *>(buf), size});
    }

    ReadCountRet UartBase::read(void *extBuffer, std::size_t size) const noexcept {
        const auto recBytes = ::read(getNativeHandler(), extBuffer, size);
        if (recBytes == -1) {
            return fromErrno<ReadCount>();
        }

        return recBytes;
    }

    Status UartBase::lockTty() noexcept {
        return (
                ((flock(getNativeHandler(), LOCK_EX | LOCK_NB) == -1) ||
                 (flock(getNativeHandler(), LOCK_UN) == -1)) ?
                fromErrno() :
                Status()
        );
    }
}