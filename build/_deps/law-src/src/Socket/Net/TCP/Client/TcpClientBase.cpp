#include <poll.h>

#include "law/RetCode/RetCode.h"
#include "law/Socket/Net/TCP/Client/TcpClientBase.h"

namespace law::socket::net::tcp::detail {
    TcpClientBase::TcpClientBase() {
        Base::openSocket();
    }

    TcpClientBase::TcpClientBase(InternetAddress &&addr) : serverAddress(std::move(addr)) {
        Base::openSocket();
    }

    TcpClientBase::TcpClientBase(const InternetAddress &addr) : serverAddress(addr) {
        Base::openSocket();
    }

    TcpClientBase::TcpClientBase(TcpClientBase &&other) noexcept:
            Base(std::move(other)),
            serverAddress(std::move(other.serverAddress)) {}

    TcpClientBase &TcpClientBase::operator=(TcpClientBase &&rhs) noexcept {
        if (&rhs != this) {
            serverAddress = std::move(rhs.serverAddress);
            Base::operator=(std::move(rhs));
        }
        return *this;
    }

    [[nodiscard]] const InternetAddress &TcpClientBase::getServerAddress() const noexcept {
        return serverAddress;
    }

    Status TcpClientBase::reconnect(std::chrono::milliseconds timeout) noexcept {
        return disconnect().and_then([this, timeout] noexcept { return connect(timeout); });
    }

    Status TcpClientBase::reconnect() noexcept {
        return disconnect().and_then([this]
        noexcept {return connect();});
    }

    Status TcpClientBase::connect(std::chrono::milliseconds timeout) const noexcept {
        auto alreadyNonblock = Base::nonblock();
        if (!alreadyNonblock) {
            return fromErrno();
        }

        if (!alreadyNonblock.value()) {
            if (!Base::nonblock(true)) {
                return fromErrno();
            }
        }

        if (connect()) {
            return {};
        }

        if (errno != EINPROGRESS) {
            if (!alreadyNonblock && !Base::nonblock(false)) {
                return fromErrno();
            }

            return fromErrno();
        }

        return waitConnect(timeout.count())
                .or_else(
                        [this, alreadyNonblock = alreadyNonblock.value()](RetCode) noexcept {
                            if (!alreadyNonblock && !Base::nonblock(false)) {
                                return fromErrno();
                            }

                            return fromLawError(RetCode::X_ECONNREFUSED);
                        }
                );
    }

    Status TcpClientBase::connect() const noexcept {
        const auto tmp = serverAddress.getNativeAddress();
        const auto addr = reinterpret_cast<const struct sockaddr *>(&tmp);
        return (::connect(getNativeHandler(), addr, sizeof(tmp)) == -1) ? fromErrno() : Status();
    }

    Status TcpClientBase::disconnect() noexcept {
        return Base::close()
                .and_then(
                    [this] noexcept {
                        return Base::makeSocket();
                    }
                );
    }

    WriteCountRet TcpClientBase::write(std::string_view data, const SendOption &so) const noexcept {
        return write(data.data(), data.size(), so);
    }

    WriteCountRet TcpClientBase::write(const void *buf, std::size_t size, const SendOption &so) const noexcept {
        auto bytesWrittenCount = ::send(getNativeHandler(), buf, size, so.getAsIntegral());
        if (bytesWrittenCount == -1) {
            return fromErrno<WriteCount>();
        }

        return bytesWrittenCount;
    }

    WriteCountRet TcpClientBase::writev(IoVectorSet set) noexcept {
        auto bytesWrittenCount = ::writev(getNativeHandler(), set.data(), int(set.size()));
        if (bytesWrittenCount == -1) {
            return fromErrno<WriteCount>();
        }

        return bytesWrittenCount;
    }

    [[nodiscard]] ReadCountRet
    TcpClientBase::read(void *extBuffer, std::size_t readCount, const ReadOption &ro) const noexcept {
        const auto recBytes = recv(getNativeHandler(), extBuffer, readCount, ro.getAsIntegral());
        if (recBytes == -1) {
            return fromErrno<ReadCount>();
        }

        return recBytes;
    }

    [[nodiscard]] SocketErrorRet TcpClientBase::getSocketError() const noexcept {
        int error{};
        return SocketBase::readOption(SocketOptionLevel::SOCKET, SocketOption::ERROR, &error)
                .transform(
                        [&error] noexcept {
                            return error;
                        }
                );
    }

    [[nodiscard]] Status TcpClientBase::waitConnect(ssize_t connectTimeoutMs) const noexcept {
        pollfd pfd{.fd = getNativeHandler(), .events = POLLOUT, .revents = 0};

        if (const auto pollResult = poll(&pfd, 1, connectTimeoutMs);
                pollResult == -1 && errno != EINTR) {
            return fromErrno();
        } else if (pollResult > 0) {
            return getSocketError()
                    .and_then(
                            [](SocketError err) noexcept {
                                if (err) {
                                    return fromLawError(RetCode(err));
                                } else {
                                    return Status();
                                }
                            }
                    );
        }

        return fromLawError(RetCode::TIMEOUT);
    }
}