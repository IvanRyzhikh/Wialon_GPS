#include <sys/signalfd.h>

#include "law/Signal/SignalfdFlags.h"

namespace law::signal {
    SignalfdFlags::SignalfdFlags(int value) noexcept: IntFlagValue(value) {}

    SignalfdFlags SignalfdFlags::operator&(const SignalfdFlags &f) const noexcept {
        return SignalfdFlags{getAsIntegral() & f.getAsIntegral()};
    }

    SignalfdFlags SignalfdFlags::operator|(const SignalfdFlags &f) const noexcept {
        return SignalfdFlags{getAsIntegral() | f.getAsIntegral()};
    }

    Nonblock::Nonblock() noexcept: SignalfdFlags(SFD_NONBLOCK) { }

    CloseOnExec::CloseOnExec() noexcept: SignalfdFlags(SFD_CLOEXEC) { }
}
