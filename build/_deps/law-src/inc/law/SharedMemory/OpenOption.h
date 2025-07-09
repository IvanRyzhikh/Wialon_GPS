#pragma once

#include "law/FlagValue.h"

namespace law::shm {
    struct OpenOption : public IntFlagValue {
        using IntFlagValue::IntFlagValue;

        OpenOption operator&(const OpenOption &of) const noexcept;

        OpenOption operator|(const OpenOption &of) const noexcept;
    };

    struct ReadOnly final : public OpenOption {
        ReadOnly() noexcept;
    };

    struct ReadWrite final : public OpenOption {
        ReadWrite() noexcept;
    };

    struct Create final : public OpenOption {
        Create() noexcept;
    };

    struct ExitIfExist final : public OpenOption {
        ExitIfExist() noexcept;
    };

    struct Truncate final : public OpenOption {
        Truncate() noexcept;
    };
}
