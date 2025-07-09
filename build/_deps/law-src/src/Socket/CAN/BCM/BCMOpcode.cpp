#include <law/Socket/CAN/BCM/BCMOpcode.h>

namespace law::socket::can {
    TxOpcode TxOpcode::operator|(const TxOpcode &oc) const noexcept {
        return TxOpcode{getAsIntegral() | oc.getAsIntegral()};
    }

    TxSetup::TxSetup() noexcept : TxOpcode(TX_SETUP) {}

    TxDelete::TxDelete() noexcept : TxOpcode(TX_DELETE) {}

    TxRead::TxRead() noexcept : TxOpcode(TX_READ) {}

    TxSend::TxSend() noexcept : TxOpcode(TX_SEND) {}

    RxOpcode RxOpcode::operator|(const RxOpcode &oc) const noexcept {
        return RxOpcode{getAsIntegral() | oc.getAsIntegral()};
    }

    RxSetup::RxSetup() noexcept : RxOpcode(RX_SETUP) {}

    RxDelete::RxDelete() noexcept : RxOpcode(RX_DELETE) {}

    RxRead::RxRead() noexcept : RxOpcode(RX_READ) {}
}
