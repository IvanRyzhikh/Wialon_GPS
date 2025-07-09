#include <fcntl.h>

#include "law/Pipe/PipeFlags.h"

namespace law::pipe {
    PipeFlags PipeFlags::operator&(const PipeFlags &f) const noexcept {
        return PipeFlags{getAsIntegral() & f.getAsIntegral()};
    }

    PipeFlags PipeFlags::operator|(const PipeFlags &f) const noexcept {
        return PipeFlags{getAsIntegral() | f.getAsIntegral()};
    }

    CloseOnExec::CloseOnExec() noexcept: PipeFlags(O_CLOEXEC) { }

    Direct::Direct() noexcept: PipeFlags(O_DIRECT) { }

    Nonblock::Nonblock() noexcept: PipeFlags(O_NONBLOCK) { }
}
