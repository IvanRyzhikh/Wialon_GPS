#include <fcntl.h>

#include "law/Semaphore/OpenOption.h"

namespace law::sem {
    OpenOption OpenOption::operator&(const OpenOption &of) const noexcept {
        return OpenOption{getAsIntegral() & of.getAsIntegral()};
    }

    OpenOption OpenOption::operator|(const OpenOption &of) const noexcept {
        return OpenOption{getAsIntegral() | of.getAsIntegral()};
    }

    Create::Create() noexcept: OpenOption(O_CREAT) { }

    ExitIfExist::ExitIfExist() noexcept: OpenOption(O_CREAT | O_EXCL) { }
}