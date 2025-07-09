#pragma once

#include "law/FlagValue.h"

namespace law::file {
    struct MappedFileSyncRule : public IntFlagValue {
        using IntFlagValue::IntFlagValue;

        MappedFileSyncRule operator&(const MappedFileSyncRule &sr) const noexcept;

        MappedFileSyncRule operator|(const MappedFileSyncRule &sr) const noexcept;
    };

    struct MappedFileAsyncSave : public MappedFileSyncRule {
        MappedFileAsyncSave() noexcept;
    };

    struct MappedFileSyncSave : public MappedFileSyncRule {
        MappedFileSyncSave() noexcept;
    };

    struct MappedFileInvalidate : public MappedFileSyncRule {
        MappedFileInvalidate() noexcept;
    };
}
