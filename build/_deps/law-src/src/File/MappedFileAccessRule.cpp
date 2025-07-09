#include <sys/mman.h>

#include "law/File/MappedFileAccessRule.h"

namespace law::file {
    MappedFileAccessRule MappedFileAccessRule::operator&(const MappedFileAccessRule &ar) const noexcept {
        return MappedFileAccessRule{getAsIntegral() & ar.getAsIntegral()};
    }

    MappedFileAccessRule MappedFileAccessRule::operator|(const MappedFileAccessRule &ar) const noexcept {
        return MappedFileAccessRule{getAsIntegral() | ar.getAsIntegral()};
    }

    MappedFileReadOnly::MappedFileReadOnly() noexcept: MappedFileAccessRule(PROT_READ) {}

    MappedFileWriteOnly::MappedFileWriteOnly() noexcept: MappedFileAccessRule(PROT_WRITE) {}

    MappedFileReadWrite::MappedFileReadWrite() noexcept: MappedFileAccessRule(PROT_READ | PROT_WRITE) {}
}