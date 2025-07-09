#pragma once

#include <csignal>

#include <law/Types.h>
#include <law/FileDescriptor/FileDescriptor.h>

namespace law::pfd {
    class Pidfd final : public fd::FileDescriptor {
    private:
        using Base = fd::FileDescriptor;

    public:
        Pidfd() noexcept = default;

        explicit Pidfd(pid_t pid);

        Status init(pid_t pid) noexcept;

        Status sendSignal(int signal, siginfo_t *info = nullptr) noexcept;

        FdRet getFd(int targetFd) noexcept;
    };
}
