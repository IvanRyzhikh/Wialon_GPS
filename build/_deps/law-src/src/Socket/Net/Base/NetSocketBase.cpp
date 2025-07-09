#include "law/Socket/Net/Base/NetSocketBase.h"

namespace law::socket::net::detail {
    Status NetSocketBase::makeSocket(socket::detail::SocketType socketType) noexcept {
        return Base::makeSocket(socket::detail::SocketDomain::INET, socketType);
    }

    Status NetSocketBase::applyBindToDevice(std::string_view networkInterface) noexcept{
        return applyOption(
                SocketOptionLevel::SOCKET,
                SocketOption::BINDTODEVICE,
                networkInterface.data());
    }
}
