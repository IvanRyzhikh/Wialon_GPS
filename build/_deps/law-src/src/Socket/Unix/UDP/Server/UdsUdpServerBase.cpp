#include <unistd.h>

#include "law/Utils.h"
#include "law/Socket/Unix/UDP/Server/UdsUdpServerBase.h"

namespace law::socket::uds::udp::detail {
    UdsUdpServerBase::UdsUdpServerBase() {
        openSocket();
    }

    UdsUdpServerBase::UdsUdpServerBase(const UnixAddress &srvAddr) : serverAddress(srvAddr) {
        openSocket();
    }

    const UnixAddress &UdsUdpServerBase::getAddress() const noexcept {
        return serverAddress;
    }

    UdsUdpServerBase::~UdsUdpServerBase() noexcept {
        if (getNativeHandler() != -1) {
            unlink(serverAddress.getPath().data());
        }
    }

    Status UdsUdpServerBase::bind() noexcept {
        auto ret = ::bind(getNativeHandler(),
                                reinterpret_cast<const sockaddr *>(&serverAddress.getNativeAddress()),
                                sizeof(sockaddr_un));
        if (ret == -1 && errno == EADDRINUSE) {
            unlink(serverAddress.getPath().data());
            ret = ::bind(getNativeHandler(),
                         reinterpret_cast<const sockaddr *>(&serverAddress.getNativeAddress()),
                         sizeof(sockaddr_un));
            if (ret == -1) {
                return fromErrno();
            }
        }

        return {};
    }

    ReadResultRet UdsUdpServerBase::read(void *extBuffer, std::size_t size, const ReadOption &ro) const noexcept {
        sockaddr_un clientAddress{};
        socklen_t fromLength = sizeof(sockaddr_un);
        const auto recBytes = recvfrom(getNativeHandler(), extBuffer, size, ro.getAsIntegral(),
                                       reinterpret_cast<sockaddr *>(&clientAddress),
                                       &fromLength);

        if (recBytes == -1) {
            return fromErrno<ReadResult>();
        }

        return ReadResult{clientAddress, std::size_t(recBytes)};
    }

    void UdsUdpServerBase::openSocket() {
        if (!makeSocket()) {
            throwErrno();
        }
    }
}
