#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>

#include <etl/string.h>
#include <etl/to_string.h>

#include "law/I2C/Base/I2CBase.h"

namespace law::i2c::detail {
    I2CBase::I2CBase(std::string_view dev) {
        if (!attach(dev)) {
            throwErrno();
        }
    }

    I2CBase::I2CBase(std::uint8_t busNumber) {
        if (!attach(busNumber)) {
            throwErrno();
        }
    }

    Status I2CBase::attach(std::uint8_t busNumber) noexcept {
        using DevPath = etl::string<sizeof("/dev/i2c-xxx") - 1>;

        static constexpr std::string_view kGenericPath = "/dev/i2c-";
        DevPath devPath(kGenericPath.data(), kGenericPath.size());

        etl::to_string(busNumber, devPath, true);
        return attach({devPath.data(), devPath.size()});
    }

    Status I2CBase::attach(std::string_view dev) noexcept {
        int fd = open(dev.data(), O_RDWR);
        if (!fd) {
            return fromErrno();
        }

        setNativeHandler(&fd);
        return {};
    }

    Status I2CBase::applyRetriesCount(std::uint32_t count) noexcept {
        return applyOption(I2C_RETRIES, count);
    }

    Status I2CBase::applyPollTimeout(PollTimeout timeout) noexcept {
        const auto tout = timeout.count();
        return applyOption(I2C_RETRIES, tout);
    }

    Status I2CBase::setSlaveAddress(std::uint16_t addr, bool force) noexcept {
        std::uint32_t address = addr; // for ioctl properly copy between userspace -> kernel

        if (force) {
            return applyOption(I2C_SLAVE_FORCE, address);
        }

        return applyOption(I2C_SLAVE, address);
    }

    Status I2CBase::tenbitAddressing(bool enable) noexcept {
        std::uint32_t enableValue = enable; // for ioctl properly copy between userspace -> kernel
        return applyOption(I2C_TENBIT, enableValue);
    }

    I2CFuncsRet I2CBase::obtainAvailableFunctions() noexcept {
        std::uint32_t ret;

        auto rc = readOption(I2C_FUNCS, &ret);
        if (!rc) {
            return std::unexpected(rc.error());
        }

        return I2CFuncs{ret};
    }

    Status I2CBase::pecUsage(bool enable) noexcept {
        std::uint32_t enableValue = enable; // for ioctl properly copy between userspace -> kernel
        return applyOption(I2C_PEC, enableValue);
    }

    WriteCountRet I2CBase::write(std::uint8_t data) noexcept {
        return write(&data, sizeof(data));
    }

    WriteCountRet I2CBase::write(std::uint16_t data) noexcept {
        return write(&data, sizeof(data));
    }

    WriteCountRet I2CBase::write(std::uint32_t data) noexcept {
        return write(&data, sizeof(data));
    }

    WriteCountRet I2CBase::write(std::string_view data) noexcept {
        return write(data.data(), data.size());
    }

    WriteCountRet I2CBase::write(const void *buffer, std::size_t size) noexcept {
        const auto rv = ::write(getNativeHandler(), buffer, size);
        if (rv == -1) {
            return std::unexpected(RetCode(errno));
        }

        return rv;
    }

    ReadCountRet I2CBase::read(void *buffer, std::size_t size) noexcept {
        const auto rv = ::read(getNativeHandler(), buffer, size);
        if (rv == -1) {
            return std::unexpected(RetCode(errno));
        }

        return rv;
    }

    Status I2CBase::transfer(I2CMsgs msgs) noexcept {
        const i2c_rdwr_ioctl_data ioctlData = {
            .msgs = msgs.data(),
            .nmsgs = msgs.size()
        };
        return applyOption(I2C_RDWR, &ioctlData);
    }
}
