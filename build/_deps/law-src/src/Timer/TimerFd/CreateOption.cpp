#include <sys/timerfd.h>

#include "law/Timer/TimerFd/CreateOption.h"

namespace law::timer::tfd {
    CreateOption CreateOption::operator&(const CreateOption &co) const noexcept {
        return CreateOption{getAsIntegral() & co.getAsIntegral()};
    }

    CreateOption CreateOption::operator|(const CreateOption &co) const noexcept {
        return CreateOption{getAsIntegral() | co.getAsIntegral()};
    }

    Nonblock::Nonblock() noexcept: CreateOption(TFD_NONBLOCK) {}

    CloseOnExec::CloseOnExec() noexcept: CreateOption(TFD_CLOEXEC) {}
}