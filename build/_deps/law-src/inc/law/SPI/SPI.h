#pragma once

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include <law/Utils.h>
#include <law/FileDescriptor/FileDescriptor.h>

#include "Types.h"

namespace law::spi {
    class SPI final : public fd::FileDescriptor {
    private:
        Mode mode;

    public:
        SPI() noexcept = default;

        explicit SPI(std::string_view devPath);

        Status open(std::string_view devPath) noexcept;

        Status applyMode(const Mode &m) noexcept;

        ModeRet getMode() noexcept;

        Status applyBitMode(BitMode bitMode) noexcept;

        BitModeRet getBitMode() noexcept;

        Status applyBitsPerWord(std::uint8_t bits) noexcept;

        BitsPerWordRet getBitsPerWord() noexcept;

        Status applyMaxSpeedHz(std::uint32_t hz) noexcept;

        SpeedHzRet getMaxSpeedHz() noexcept;

        Status transfer(spi_ioc_transfer tr) noexcept;
    };
}
