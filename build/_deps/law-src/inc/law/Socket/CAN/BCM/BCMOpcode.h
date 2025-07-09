#pragma once

#include <linux/can/bcm.h>

#include <law/FlagValue.h>

namespace law::socket::can {
    struct TxOpcode : public UInt32FlagValue {
        using UInt32FlagValue::UInt32FlagValue;

        TxOpcode operator|(const TxOpcode &oc) const noexcept;
    };

    struct TxSetup final : public TxOpcode {
        TxSetup() noexcept;
    };

    struct TxDelete final : public TxOpcode {
        TxDelete() noexcept;
    };

    struct TxRead final : public TxOpcode {
        TxRead() noexcept;
    };

    struct TxSend final : public TxOpcode {
        TxSend() noexcept;
    };

    struct RxOpcode : public UInt32FlagValue {
        using UInt32FlagValue::UInt32FlagValue;

        RxOpcode operator|(const RxOpcode &oc) const noexcept;
    };

    struct RxSetup final : public RxOpcode {
        RxSetup() noexcept;
    };

    struct RxDelete final : public RxOpcode {
        RxDelete() noexcept;
    };

    struct RxRead final : public RxOpcode {
        RxRead() noexcept;
    };
}
