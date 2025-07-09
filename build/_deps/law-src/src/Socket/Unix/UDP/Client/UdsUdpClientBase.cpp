#include <unistd.h>

#include "law/Utils.h"
#include "law/RetCode/RetCode.h"
#include "law/Socket/Unix/UDP/Client/UdsUdpClientBase.h"

namespace law::socket::uds::udp::detail {
    UdsUdpClientBase::UdsUdpClientBase() {
        openSocket();
    }

    UdsUdpClientBase::UdsUdpClientBase(const UnixAddress &srvAddr) : serverAddress(srvAddr) {
        openSocket();
    }

    UdsUdpClientBase::~UdsUdpClientBase() noexcept {
        if (operator bool()) {
            unlink(selfAddress.getPath().data());
        }
    }

    [[nodiscard]] const UnixAddress &UdsUdpClientBase::getServerAddress() const noexcept {
        return serverAddress;
    }

    void UdsUdpClientBase::setServerAddress(const UnixAddress &addr) noexcept {
        serverAddress = addr;
    }

    WriteCountRet UdsUdpClientBase::write(std::string_view data) const noexcept {
        return detail::UdsUdpSocketBase::write(data, serverAddress);
    }

    [[nodiscard]] WriteCountRet UdsUdpClientBase::write(const msghdr &message) const noexcept {
        return detail::UdsUdpSocketBase::write(message, serverAddress);
    }

    ReadCountRet UdsUdpClientBase::read(void *extBuffer, std::size_t size, const ReadOption &ro) const noexcept {
        const auto recBytes = recv(getNativeHandler(), extBuffer, size, ro.getAsIntegral());
        if (recBytes == -1) {
            return fromErrno<ReadCount>();
        }

        return recBytes;
    }

    void UdsUdpClientBase::openSocket() {
        if (!makeSocket()) {
            throwErrno();
        }

        const auto path = makeRandomUnixPath();
        selfAddress.setPath({path.data(), path.size()});

        if (bind(getNativeHandler(),
                 reinterpret_cast<const sockaddr *>(&selfAddress.getNativeAddress()),
                 sizeof(sockaddr_un)) == -1) {
            throwErrno();
        }
    }
}
