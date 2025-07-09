#pragma once

#include <net/if.h>
#include <law/Socket/CAN/CANTypes.h>
#include <law/Socket/Base/SocketBase.h>

namespace law::socket::can::detail {
    class CANBase : public socket::detail::SocketBase {
    private:
        using SocketBase::makeSocket;

    protected:
        UInt32Ret obtainCanIfIndex(ifreq &ifr, std::string_view canIfName) noexcept;

        CANAddressRet obtainCurrentAddress() noexcept;

        Status makeSocket(socket::detail::SocketType socketType, int protocol = 0) noexcept;
    };
}
