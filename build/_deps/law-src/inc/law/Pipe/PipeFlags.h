#pragma once

#include "law/FlagValue.h"

#include "law/RetCode/RetCode.h"

namespace law::pipe {
    struct PipeFlags : public IntFlagValue {
        using IntFlagValue::IntFlagValue;

        PipeFlags operator&(const PipeFlags &f) const noexcept;

        PipeFlags operator|(const PipeFlags &f) const noexcept;
    };

    struct CloseOnExec final : public PipeFlags {
        CloseOnExec() noexcept;
    };

    struct Direct final : public PipeFlags {
        Direct() noexcept;
    };

    struct Nonblock final : public PipeFlags {
        Nonblock() noexcept;
    };

    using PipeSize = std::size_t;
    using PipeSizeRet = CustomError<PipeSize>;
}
