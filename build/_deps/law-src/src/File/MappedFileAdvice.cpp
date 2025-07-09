#include <sys/mman.h>

#include "law/File/MappedFileAdvice.h"

namespace law::file {
    MappedFileAdvice MappedFileAdvice::operator&(const MappedFileAdvice &a) const noexcept {
        return MappedFileAdvice{getAsIntegral() & a.getAsIntegral()};
    }

    MappedFileAdvice MappedFileAdvice::operator|(const MappedFileAdvice &a) const noexcept {
        return MappedFileAdvice{getAsIntegral() | a.getAsIntegral()};
    }

    MappedFileAdviceNormal::MappedFileAdviceNormal() noexcept: MappedFileAdvice(MADV_NORMAL) {}

    MappedFileAdviceRandomAccess::MappedFileAdviceRandomAccess() noexcept: MappedFileAdvice(MADV_RANDOM) {}

    MappedFileAdviceSequentialAccess::MappedFileAdviceSequentialAccess() noexcept:
            MappedFileAdvice(MADV_SEQUENTIAL) {}

    MappedFileAdviceWillNeed::MappedFileAdviceWillNeed() noexcept: MappedFileAdvice(MADV_WILLNEED) {}

    MappedFileAdviceDontNeed::MappedFileAdviceDontNeed() noexcept: MappedFileAdvice(MADV_DONTNEED) {}
}
