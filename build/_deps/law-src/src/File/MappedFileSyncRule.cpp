#include <sys/mman.h>

#include "law/File/MappedFileSyncRule.h"

namespace law::file {
    MappedFileSyncRule MappedFileSyncRule::operator&(const MappedFileSyncRule &sr) const noexcept {
        return MappedFileSyncRule{getAsIntegral() & sr.getAsIntegral()};
    }

    MappedFileSyncRule MappedFileSyncRule::operator|(const MappedFileSyncRule &sr) const noexcept {
        return MappedFileSyncRule{getAsIntegral() | sr.getAsIntegral()};
    }

    MappedFileAsyncSave::MappedFileAsyncSave() noexcept: MappedFileSyncRule(MS_ASYNC) {}

    MappedFileSyncSave::MappedFileSyncSave() noexcept: MappedFileSyncRule(MS_SYNC) {}

    MappedFileInvalidate::MappedFileInvalidate() noexcept: MappedFileSyncRule(MS_INVALIDATE) {}
}