#include <cstring>

#include <law/Socket/CAN/Base/CANBase.h>

namespace law::socket::can::detail {
    UInt32Ret CANBase::obtainCanIfIndex(ifreq &ifr, std::string_view canIfName) noexcept {
        if (canIfName.empty()) {
            static constexpr int kAllCanIfaceIndex = 0;
            return kAllCanIfaceIndex;
        }

        std::strncpy(ifr.ifr_name, canIfName.data(), (IFNAMSIZ - 1));
        if (ioctl(getNativeHandler(), SIOCGIFINDEX, &ifr) == -1) {
            return fromErrno<UInt32>();
        }

        return ifr.ifr_ifindex;
    }

    CANAddressRet CANBase::obtainCurrentAddress() noexcept {
        sockaddr_can addr;
        socklen_t len = sizeof(addr);

        if (getsockname(getNativeHandler(), reinterpret_cast<sockaddr *>(&addr), &len) == -1) {
            return fromErrno<CANAddress>();
        }

        return addr;
    }

    Status CANBase::makeSocket(socket::detail::SocketType socketType, int protocol) noexcept {
        using namespace socket::detail;
        return SocketBase::makeSocket(SocketDomain::CAN, socketType, protocol);
    }
}
