#include <fcntl.h>

#include "law/SharedMemory/OpenOption.h"

namespace law::shm {
    OpenOption OpenOption::operator&(const OpenOption &of) const noexcept {
        return OpenOption{getAsIntegral() & of.getAsIntegral()};
    }

    OpenOption OpenOption::operator|(const OpenOption &of) const noexcept {
        return OpenOption{getAsIntegral() | of.getAsIntegral()};
    }

    ReadOnly::ReadOnly() noexcept: OpenOption(O_RDONLY) { }

    ReadWrite::ReadWrite() noexcept: OpenOption(O_RDWR) { }

    Create::Create() noexcept: OpenOption(O_CREAT) { }

    ExitIfExist::ExitIfExist() noexcept: OpenOption(O_EXCL) { }

    Truncate::Truncate() noexcept: OpenOption(O_TRUNC) { }
}
