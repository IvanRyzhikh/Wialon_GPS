#pragma once

#include "law/FlagValue.h"

namespace law::sem {
    struct OpenOption : public IntFlagValue {
        using IntFlagValue::IntFlagValue;

        OpenOption operator&(const OpenOption &of) const noexcept;

        OpenOption operator|(const OpenOption &of) const noexcept;
    };

    struct Create final : public OpenOption {
        Create() noexcept;
    };

    struct ExitIfExist final : public OpenOption {
        ExitIfExist() noexcept;
    };
}
