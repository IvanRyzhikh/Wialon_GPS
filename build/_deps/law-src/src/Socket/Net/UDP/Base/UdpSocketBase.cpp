#include "law/Socket/Net/UDP/Base/UdpSocketBase.h"

namespace law::socket::net::udp::detail {
    void UdpSocketBase::openSocket() {
        if (!makeSocket()) {
            throwErrno();
        }
    }

    Status UdpSocketBase::makeSocket() noexcept {
        return Base::makeSocket(socket::detail::SocketType::DGRAM);
    }
}