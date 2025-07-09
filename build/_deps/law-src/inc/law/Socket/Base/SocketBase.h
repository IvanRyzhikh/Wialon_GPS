#pragma once

#include <chrono>

#include "SocketType.h"
#include "SocketDomain.h"
#include "SocketOption.h"
#include "SocketOptionLevel.h"

#include "law/Types.h"
#include "law/RetCode/RetCode.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::socket::detail {
    class SocketBase : public fd::FileDescriptor {
    protected:
        static constexpr std::size_t kDefaultReadCount = 8192;

    public:
        Status applyOption(SocketOptionLevel level, SocketOption socketOption, const auto *optionValue) const noexcept {
            return applyOption(level, static_cast<int>(socketOption), optionValue);
        }

        Status readOption(SocketOptionLevel level, SocketOption socketOption, auto *optionValue) const noexcept {
            return readOption(level, static_cast<int>(socketOption), optionValue);
        }

        Status setReadTimeout(std::chrono::microseconds timeout) noexcept;

        Status setWriteTimeout(std::chrono::microseconds timeout) noexcept;

        Status resetReadTimeout() noexcept;

        Status resetWriteTimeout() noexcept;

    protected:
        Status makeSocket(SocketDomain socketDomain, SocketType socketType, int protocol = 0) noexcept;

        Status applyOption(
            SocketOptionLevel level, auto socketOption,
            const auto *optionValue, std::size_t optionLength = sizeof(*optionValue)) const noexcept {
            return LAW_INTERNAL_ERROR_CHECK(
                setsockopt(
                    getNativeHandler(), static_cast<int>(level),
                    static_cast<int>(socketOption), optionValue, optionLength
                )
            );
        }

        Status readOption(SocketOptionLevel level, auto socketOption, auto *optionValue) const noexcept {
            socklen_t valueSize = sizeof(*optionValue);
            return (
                       getsockopt(
                           getNativeHandler(), static_cast<int>(level),
                           static_cast<int>(socketOption), optionValue, &valueSize
                       ) == -1
                   )
                       ? fromErrno()
                       : Status();
        }

    private:
        Status setTimeout(std::chrono::microseconds timeout, SocketOption type) const noexcept;
    };
}
