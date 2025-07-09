#include <fcntl.h>
#include <sys/time.h>

#include "law/Socket/Base/SocketBase.h"

namespace law::socket::detail {
    Status SocketBase::setReadTimeout(std::chrono::microseconds timeout) noexcept {
        return setTimeout(timeout, SocketOption::RCVTIMEO);
    }

    Status SocketBase::setWriteTimeout(std::chrono::microseconds timeout) noexcept {
        return setTimeout(timeout, SocketOption::SNDTIMEO);
    }

    Status SocketBase::resetReadTimeout() noexcept {
        return setTimeout(std::chrono::microseconds(0), SocketOption::RCVTIMEO);
    }

    Status SocketBase::resetWriteTimeout() noexcept {
        return setTimeout(std::chrono::microseconds(0), SocketOption::SNDTIMEO);
    }

    Status SocketBase::makeSocket(SocketDomain socketDomain, SocketType socketType, int protocol) noexcept {
        if (!operator bool()) {
            auto fd = ::socket(int(socketDomain), int(socketType), protocol);
            setNativeHandler(&fd);
            return ((operator bool()) ? Status() : fromErrno());
        } else {
            return {};
        }
    }

    Status SocketBase::setTimeout(std::chrono::microseconds timeout, SocketOption type) const noexcept {
        using namespace std::chrono;

        timeval tv{};
        if (timeout < 1s) {
            tv.tv_sec = 0;
            tv.tv_usec = long(timeout.count());
        } else {
            auto chronoSec = duration_cast<seconds>(timeout);
            tv.tv_sec = time_t(chronoSec.count());
            tv.tv_usec = long((duration_cast<microseconds>(chronoSec) - duration_cast<microseconds>(timeout)).count());
        }

        return applyOption(SocketOptionLevel::SOCKET, type, &tv);
    }
}