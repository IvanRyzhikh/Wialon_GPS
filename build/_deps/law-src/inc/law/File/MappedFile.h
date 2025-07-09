#pragma once

#include <span>

#include "File.h"
#include "law/Types.h"
#include "MappedFileAdvice.h"
#include "MappedFileSyncRule.h"
#include "MappedFileAccessRule.h"
#include "MappedFileVisibilityRule.h"

namespace law::file {
    class MappedFile final {
    private:
        char *pMappedFile = nullptr;
        std::size_t mappedFileSize = 0;

    public:
        MappedFile() noexcept = default;

        MappedFile(std::string_view filename,
                   const MappedFileAccessRule &ar = MappedFileReadWrite(),
                   MappedFileVisibilityRule vr = MappedFileVisibilityRule::SHARED) noexcept;

        MappedFile(MappedFile &&other) noexcept;

        MappedFile(const MappedFile &) = delete;

        ~MappedFile() noexcept;

        MappedFile &operator=(MappedFile &&rhs) noexcept;

        MappedFile &operator=(const MappedFile &) = delete;

        Status createMapping(std::string_view filename,
                             const MappedFileAccessRule &ar = MappedFileReadWrite(),
                             MappedFileVisibilityRule vr = MappedFileVisibilityRule::SHARED) noexcept;

        Status flush(const MappedFileSyncRule &sr = MappedFileSyncSave()) noexcept;

        Status acceptAdvise(const MappedFileAdvice &a) noexcept;

        std::span<char> get() noexcept;
    };
}
