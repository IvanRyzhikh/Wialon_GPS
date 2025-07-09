#pragma once

#include <csignal>
#include <sys/signalfd.h>

#include "law/Utils.h"
#include "SignalfdFlags.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::signal {
    class Signalfd final : public fd::FileDescriptor {
    public:
        using SiginfoRet = CustomError<signalfd_siginfo>;

    private:
        using SigsetRet = CustomError<sigset_t>;

    private:
        sigset_t sigset;

    public:
        Signalfd() noexcept = default;

        template <std::same_as<std::uint32_t> ...Signals>
        explicit Signalfd(Signals ...signals, const SignalfdFlags &flags = SignalfdFlags(0)) {
            if (!init(signals..., flags)) {
                throwErrno();
            }
        }

        ~Signalfd() noexcept override;

        template <std::integral ...Signals>
        Status init(std::uint32_t signalHead, Signals ...signals) noexcept {
            return makeSigset(signalHead, signals...)
                    .and_then(
                            [this](sigset_t mask) noexcept {
                                sigset = mask;
                                return maskSigset(SIG_BLOCK, sigset);
                            }
                    )
                    .and_then(
            [this]
            noexcept {
                    int fd = signalfd(-1, &sigset, 0);
                    setNativeHandler(&fd);
                    if (!operator bool()) {
                        return fromErrno();
                    }

                    return Status();
            }
            );
        }

        template <std::integral ...Signals>
        Status init(const SignalfdFlags &flags, std::uint32_t signalHead, Signals ...signals) noexcept {
            return makeSigset(signalHead, signals...)
                    .and_then(
                            [this](sigset_t mask) noexcept {
                                sigset = mask;
                                return maskSigset(SIG_BLOCK, sigset);
                            }
                    )
                    .and_then(
                            [this, &flags] noexcept {
                                int fd = signalfd(-1, &sigset, flags.getAsIntegral());
                                setNativeHandler(&fd);
                                if (!operator bool()) {
                                    return fromErrno();
                                }

                                return Status();
                            }
                    );
        }

        template <std::integral ...Signals>
        Status update(Signals ...signals) noexcept {
            if (!operator bool()) {
                return fromLawError(RetCode::X_EBADF);
            }

            return maskSigset(SIG_UNBLOCK, sigset)
                    .and_then(
                            [this, &signals...] noexcept {
                                return makeSigset(signals...);
                            }
                    )
                    .and_then(
                            [this](sigset_t mask) noexcept {
                                sigset = mask;
                                return maskSigset(SIG_BLOCK, sigset);
                            }
                    )
                    .and_then(
                            [this] noexcept {
                                LAW_INTERNAL_ERROR_CHECK(signalfd(getNativeHandler(), &sigset, 0));
                            }
                    );
        }

        [[nodiscard]] SiginfoRet read() const noexcept;

    private:
        template <std::integral ...Signals>
        static SigsetRet makeSigset(Signals ...signals) noexcept {
            sigset_t mask;
            if (sigemptyset(&mask) == -1) {
                return fromErrno<sigset_t>();
            }

            (sigaddset(&mask, signals), ...);
            return mask;
        }

        static Status maskSigset(int action, const sigset_t &sigset) noexcept;
    };
}
