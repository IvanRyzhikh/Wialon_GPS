#include <linux/spi/spidev.h>

#include <law/SPI/Mode.h>

namespace law::spi {
    Mode Mode::operator|(const Mode &m) const noexcept {
        return Mode{getAsIntegral() | m.getAsIntegral()};
    }

    Mode0::Mode0() noexcept: Mode(SPI_MODE_0) {}

    Mode1::Mode1() noexcept: Mode(SPI_MODE_1) {}

    Mode2::Mode2() noexcept: Mode(SPI_MODE_2) {}

    Mode3::Mode3() noexcept: Mode(SPI_MODE_3) {}

    ChipSelectActiveHigh::ChipSelectActiveHigh() noexcept: Mode(SPI_CS_HIGH) {}

    LSBFirst::LSBFirst() noexcept : Mode(SPI_LSB_FIRST) { }

    SharedSiSo::SharedSiSo() noexcept : Mode(SPI_3WIRE) { }

    Loopback::Loopback() noexcept : Mode(SPI_LOOP) { }

    NoChipSelect::NoChipSelect() noexcept : Mode(SPI_NO_CS) { }

    Ready::Ready() noexcept : Mode(SPI_READY) { }

    TxDualTransfer::TxDualTransfer() noexcept : Mode(SPI_TX_DUAL) { }

    TxQuadTransfer::TxQuadTransfer() noexcept : Mode(SPI_TX_QUAD) { }

    RxDualTransfer::RxDualTransfer() noexcept : Mode(SPI_RX_DUAL) { }

    RxQuadTransfer::RxQuadTransfer() noexcept : Mode(SPI_RX_QUAD) { }
}
