#pragma once

#include "law/FlagValue.h"

namespace law::file {
    struct MappedFileAccessRule : public IntFlagValue {
        using IntFlagValue::IntFlagValue;

        MappedFileAccessRule operator&(const MappedFileAccessRule &ar) const noexcept;

        MappedFileAccessRule operator|(const MappedFileAccessRule &ar) const noexcept;
    };

    struct MappedFileReadOnly final : public MappedFileAccessRule {
        MappedFileReadOnly() noexcept;
    };

    struct MappedFileWriteOnly final : public MappedFileAccessRule {
        MappedFileWriteOnly() noexcept;
    };

    struct MappedFileReadWrite final : public MappedFileAccessRule {
        MappedFileReadWrite() noexcept;
    };
}
