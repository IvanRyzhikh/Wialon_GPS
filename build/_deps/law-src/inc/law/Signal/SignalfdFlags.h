#pragma once

#include "law/FlagValue.h"

namespace law::signal {
    struct SignalfdFlags : public IntFlagValue {
        explicit SignalfdFlags(int value) noexcept;

        SignalfdFlags operator&(const SignalfdFlags &f) const noexcept;

        SignalfdFlags operator|(const SignalfdFlags &f) const noexcept;
    };

    struct Nonblock final : public SignalfdFlags {
        Nonblock() noexcept;
    };

    struct CloseOnExec final : public SignalfdFlags {
        CloseOnExec() noexcept;
    };
}