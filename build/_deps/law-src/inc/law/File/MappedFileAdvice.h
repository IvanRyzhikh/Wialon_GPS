#pragma once

#include "law/FlagValue.h"

namespace law::file {
    struct MappedFileAdvice : public IntFlagValue {
        using IntFlagValue::IntFlagValue;

        MappedFileAdvice operator&(const MappedFileAdvice &a) const noexcept;

        MappedFileAdvice operator|(const MappedFileAdvice &a) const noexcept;
    };

    struct MappedFileAdviceNormal : public MappedFileAdvice {
        MappedFileAdviceNormal() noexcept;
    };

    struct MappedFileAdviceRandomAccess : public MappedFileAdvice {
        MappedFileAdviceRandomAccess() noexcept;
    };

    struct MappedFileAdviceSequentialAccess : public MappedFileAdvice {
        MappedFileAdviceSequentialAccess() noexcept;
    };

    struct MappedFileAdviceWillNeed : public MappedFileAdvice {
        MappedFileAdviceWillNeed() noexcept;
    };

    struct MappedFileAdviceDontNeed : public MappedFileAdvice {
        MappedFileAdviceDontNeed() noexcept;
    };
}
