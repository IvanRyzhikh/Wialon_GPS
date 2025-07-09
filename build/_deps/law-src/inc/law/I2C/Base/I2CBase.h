#pragma once

#include <law/Types.h>
#include <law/I2C/Types.h>
#include <law/FileDescriptor/FileDescriptor.h>

namespace law::i2c::detail {
    class I2CBase : public fd::FileDescriptor {
    public:
        I2CBase() noexcept = default;

        I2CBase(std::string_view dev);

        I2CBase(std::uint8_t busNumber);

        Status attach(std::uint8_t busNumber) noexcept;

        Status attach(std::string_view dev) noexcept;

        Status applyRetriesCount(std::uint32_t count) noexcept;

        Status applyPollTimeout(PollTimeout timeout) noexcept;

        Status setSlaveAddress(std::uint16_t addr, bool force = false) noexcept;

        Status tenbitAddressing(bool enable) noexcept;

        I2CFuncsRet obtainAvailableFunctions() noexcept;

        Status pecUsage(bool enable) noexcept;

        WriteCountRet write(std::uint8_t data) noexcept;

        WriteCountRet write(std::uint16_t data) noexcept;

        WriteCountRet write(std::uint32_t data) noexcept;

        WriteCountRet write(std::string_view data) noexcept;

        WriteCountRet write(const void *buffer, std::size_t size) noexcept;

        ReadCountRet read(void *buffer, std::size_t size) noexcept;

        Status transfer(I2CMsgs msgs) noexcept;
    };
}
