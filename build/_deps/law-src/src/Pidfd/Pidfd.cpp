#include <unistd.h>

#include <law/Pidfd/Pidfd.h>

#ifndef SYS_pidfd_open
#define SYS_pidfd_open 434
#endif

#ifndef SYS_pidfd_getfd
#define SYS_pidfd_getfd 438
#endif

#ifndef SYS_pidfd_send_signal
#define SYS_pidfd_send_signal 424
#endif

static int pidfd_open(pid_t pid, unsigned int flags) {
    return syscall(SYS_pidfd_open, pid, flags);
}

static int pidfd_getfd(int pidfd, int targetfd, unsigned int flags) {
    return syscall(SYS_pidfd_getfd, pidfd, targetfd, flags);
}

static int pidfd_send_signal(int pidfd, int sig, siginfo_t *info, unsigned int flags) {
    return syscall(SYS_pidfd_send_signal, pidfd, sig, info, flags);
}

namespace law::pfd {
    Pidfd::Pidfd(pid_t pid) : Base(pidfd_open(pid, 0)) {
        if (!operator bool()) {
            throwErrno();
        }
    }

    Status Pidfd::init(pid_t pid) noexcept {
        auto fd = pidfd_open(pid, 0);
        if (fd == -1) {
            return fromErrno();
        }

        setNativeHandler(&fd);

        return {};
    }

    Status Pidfd::sendSignal(int signal, siginfo_t *info) noexcept {
        return ((pidfd_send_signal(getNativeHandler(), signal, info, 0) == -1) ? fromErrno() : Status());
    }

    FdRet Pidfd::getFd(int targetFd) noexcept {
        const auto ret = pidfd_getfd(getNativeHandler(), targetFd, 0);
        if (ret == -1) {
            return fromErrno<Fd>();
        }

        return ret;
    }
}
