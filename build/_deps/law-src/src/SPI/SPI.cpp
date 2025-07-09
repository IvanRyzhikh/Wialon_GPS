#include <law/SPI/SPI.h>

namespace law::spi {
    SPI::SPI(std::string_view devPath) {
        if (!open(devPath)) {
            throw std::system_error(errno, std::system_category());
        }
    }

    Status SPI::open(std::string_view devPath) noexcept {
        auto fd = ::open(devPath.data(), O_RDWR);
        if (fd == -1) {
            return fromErrno();
        }

        setNativeHandler(&fd);
        return getMode()
                .and_then(
                        [this](const Mode &m) noexcept {
                            mode = m;
                            return Status();
                        }
                );
    }

    Status SPI::applyMode(const Mode &m) noexcept {
        const auto modeVal = m.getAsIntegral();
        if (ioctl(getNativeHandler(), SPI_IOC_WR_MODE32, &modeVal) == -1) {
            return fromErrno();
        }

        mode = m;
        return {};
    }

    ModeRet SPI::getMode() noexcept {
        std::uint32_t m;
        if (ioctl(getNativeHandler(), SPI_IOC_RD_MODE32, &m) == -1) {
            return fromErrno<Mode>();
        }

        mode = Mode{m};
        return mode;
    }

    Status SPI::applyBitMode(BitMode bitMode) noexcept {
        return LAW_INTERNAL_ERROR_CHECK(ioctl(getNativeHandler(), SPI_IOC_WR_LSB_FIRST, &bitMode));
    }

    BitModeRet SPI::getBitMode() noexcept {
        BitMode bitMode;
        if (ioctl(getNativeHandler(), SPI_IOC_RD_LSB_FIRST, &bitMode) == -1) {
            return fromErrno<BitMode>();
        }

        return bitMode;
    }

    Status SPI::applyBitsPerWord(std::uint8_t bits) noexcept {
        return LAW_INTERNAL_ERROR_CHECK(ioctl(getNativeHandler(), SPI_IOC_WR_BITS_PER_WORD, &bits));
    }

    BitsPerWordRet SPI::getBitsPerWord() noexcept {
        std::uint8_t bits;
        if (ioctl(getNativeHandler(), SPI_IOC_RD_BITS_PER_WORD, &bits) == -1) {
            return fromErrno<BitsPerWord>();
        }

        return bits;
    }

    Status SPI::applyMaxSpeedHz(std::uint32_t hz) noexcept {
        return LAW_INTERNAL_ERROR_CHECK(ioctl(getNativeHandler(), SPI_IOC_WR_MAX_SPEED_HZ, &hz));
    }

    SpeedHzRet SPI::getMaxSpeedHz() noexcept {
        std::uint32_t bits;
        if (ioctl(getNativeHandler(), SPI_IOC_RD_MAX_SPEED_HZ, &bits) == -1) {
            return fromErrno<SpeedHz>();
        }

        return bits;
    }

    Status SPI::transfer(spi_ioc_transfer tr) noexcept {
        if (mode & TxQuadTransfer()) {
            tr.tx_nbits = 4;
        } else if (mode & TxDualTransfer()) {
            tr.tx_nbits = 2;
        }

        if (mode & RxDualTransfer()) {
            tr.rx_nbits = 4;
        } else if (mode & RxQuadTransfer()) {
            tr.rx_nbits = 2;
        }

        if (!(mode & Loopback())) {
            if (mode & (TxQuadTransfer() | RxDualTransfer())) {
                tr.rx_buf = 0;
            } else if (mode & (RxQuadTransfer() | RxDualTransfer())) {
                tr.tx_buf = 0;
            }
        }

        return LAW_INTERNAL_ERROR_CHECK(ioctl(getNativeHandler(), SPI_IOC_MESSAGE(1), &tr));
    }
}
