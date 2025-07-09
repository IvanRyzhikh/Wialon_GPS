#include "law/RetCode/RetCode.h"
#include "law/Socket/Net/TCP/Base/TcpSocketBase.h"

namespace law::socket::net::tcp::detail {
    void TcpSocketBase::openSocket() {
        if (!makeSocket()) {
            throwErrno();
        }
    }

    Status TcpSocketBase::makeSocket() noexcept {
        return Base::makeSocket(socket::detail::SocketType::STREAM);
    }

    Status TcpSocketBase::setupKeepalive(
            std::chrono::seconds startThreshold,
            std::uint32_t repeats,
            std::chrono::seconds delayBetweenRepeats) const noexcept {
        static constexpr int enable = 1;
        const auto trafficMaxDelaySec = startThreshold.count();
        const auto keepaliveRetryGapSec = delayBetweenRepeats.count();
        const auto connectionTimeout = trafficMaxDelaySec + (repeats * keepaliveRetryGapSec);
        return Base::applyOption(SocketOptionLevel::SOCKET, SocketOption::KEEPALIVE, &enable)
                .and_then(
                        [this, trafficMaxDelaySec] noexcept {
                            return applyOption(SocketOptionLevel::TCP, TcpOption::KEEPIDLE, &trafficMaxDelaySec);
                        }
                )
                .and_then(
                        [this, repeats] noexcept {
                            return applyOption(SocketOptionLevel::TCP, TcpOption::KEEPCNT, &repeats);
                        }
                )
                .and_then(
                        [this, keepaliveRetryGapSec] noexcept {
                            return applyOption(SocketOptionLevel::TCP, TcpOption::KEEPINTVL, &keepaliveRetryGapSec);
                        }
                )
                .and_then(
                        [this, connectionTimeout] noexcept {
                            return applyOption(SocketOptionLevel::TCP, TcpOption::USER_TIMEOUT, &connectionTimeout);
                        }
                );
    }

    Status TcpSocketBase::disableNagleAlgorithm() const noexcept {
        static constexpr int enable = 1;
        return applyOption(SocketOptionLevel::TCP, TcpOption::NODELAY, &enable);
    }

    Status TcpSocketBase::reuseAddress() const noexcept {
        static constexpr int enable = 1;
        return Base::applyOption(SocketOptionLevel::SOCKET, SocketOption::REUSEADDR, &enable);
    }

    Status TcpSocketBase::reusePort() const noexcept {
        static constexpr int enable = 1;
        return Base::applyOption(SocketOptionLevel::SOCKET, SocketOption::REUSEPORT, &enable);
    }

    TcpInfoRet TcpSocketBase::getTcpInfo() const noexcept {
        tcp_info ret{};
        return readOption(SocketOptionLevel::TCP, TcpOption::INFO, &ret)
                .transform(
                        [&ret] noexcept {
                            return ret;
                        }
                );
    }
}