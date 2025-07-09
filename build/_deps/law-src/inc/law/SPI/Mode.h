#pragma once

#include <law/FlagValue.h>

namespace law::spi {
    struct Mode : public UInt32FlagValue {
        using UInt32FlagValue::UInt32FlagValue;

        Mode operator|(const Mode &m) const noexcept;
    };

    struct Mode0 final : public Mode {
        Mode0() noexcept;
    };

    struct Mode1 final : public Mode {
        Mode1() noexcept;
    };

    struct Mode2 final : public Mode {
        Mode2() noexcept;
    };

    struct Mode3 final : public Mode {
        Mode3() noexcept;
    };

    struct ChipSelectActiveHigh final : public Mode {
        ChipSelectActiveHigh() noexcept;
    };

    struct LSBFirst final : public Mode {
        LSBFirst() noexcept;
    };

    struct SharedSiSo final : public Mode {
        SharedSiSo() noexcept;
    };

    struct Loopback final : public Mode {
        Loopback() noexcept;
    };

    struct NoChipSelect final : public Mode {
        NoChipSelect() noexcept;
    };

    struct Ready final : public Mode {
        Ready() noexcept;
    };

    struct TxDualTransfer final : public Mode {
        TxDualTransfer() noexcept;
    };

    struct TxQuadTransfer final : public Mode {
        TxQuadTransfer() noexcept;
    };

    struct RxDualTransfer final : public Mode {
        RxDualTransfer() noexcept;
    };

    struct RxQuadTransfer final : public Mode {
        RxQuadTransfer() noexcept;
    };
}
