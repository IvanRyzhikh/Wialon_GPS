#pragma once

#include <span>
#include <chrono>
#include <linux/i2c.h>

namespace law::i2c {
    using PollTimeout = std::chrono::duration<std::uint32_t, std::centi>;

    union __attribute__((packed)) I2CFuncs {
        std::uint32_t value;

        struct {
            std::uint32_t unused : 3;
            std::uint32_t smbusHostNotify: 1;
            std::uint32_t smbusWriteI2CBlock : 1;
            std::uint32_t smbusReadI2CBlock : 1;
            std::uint32_t smbusWriteBlockData : 1;
            std::uint32_t smbusReadBlockData : 1;
            std::uint32_t smbusProcCall : 1;
            std::uint32_t smbusWriteWordData : 1;
            std::uint32_t smbusReadWordData : 1;
            std::uint32_t smbusWriteByteData : 1;
            std::uint32_t smbusReadByteData : 1;
            std::uint32_t smbusWriteByte : 1;
            std::uint32_t smbusReadByte : 1;
            std::uint32_t smbusQuick : 1;
            std::uint32_t smbusBlockProcCall : 1;
            std::uint32_t slave : 1;
            std::uint32_t noStart : 1;
            std::uint32_t smbusPEC : 1;
            std::uint32_t protocolMangling : 1;
            std::uint32_t tenbitAddr : 1;
            std::uint32_t i2c : 1;
        };
    };
    using I2CFuncsRet = CustomError<I2CFuncs>;

    using I2CMsgs = std::span<i2c_msg>;
}
