#include <unistd.h>

#include <boost/assert.hpp>

#include "law/Signal/Signalfd.h"

namespace law::signal {
    Signalfd::~Signalfd() noexcept {
        if (operator bool()) {
            BOOST_VERIFY(maskSigset(SIG_UNBLOCK, sigset));
        }
    }

    Signalfd::SiginfoRet Signalfd::read() const noexcept {
        signalfd_siginfo siginfo{};
        if (::read(getNativeHandler(), &siginfo, sizeof(signalfd_siginfo)) != sizeof(signalfd_siginfo)) {
            return fromErrno<signalfd_siginfo>();
        }

        return siginfo;
    }

    Status Signalfd::maskSigset(int action, const sigset_t &sigset) noexcept {
        return LAW_INTERNAL_ERROR_CHECK(sigprocmask(action, &sigset, nullptr));
    }
}
