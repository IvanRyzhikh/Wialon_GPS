#include "law/Utils.h"
#include "law/Socket/Net/UDP/Client/UdpClientBase.h"

namespace law::socket::net::udp::detail {
    UdpClientBase::UdpClientBase() {
        Base::openSocket();
    }

    UdpClientBase::UdpClientBase(const InternetAddress &srvAddr) : serverAddress(srvAddr) {
        Base::openSocket();
    }

    void UdpClientBase::setServerAddress(const InternetAddress &addr) noexcept {
        serverAddress = addr;
    }

    [[nodiscard]] const InternetAddress &UdpClientBase::getServerAddress() const noexcept {
        return serverAddress;
    }

    WriteCountRet UdpClientBase::write(std::string_view data, const law::socket::SendOption &so) const noexcept {
        return write(data.data(), data.size(), so);
    }

    WriteCountRet UdpClientBase::write(const void *buf, std::size_t size, const SendOption &so) const noexcept {
        const auto addr = serverAddress.getNativeAddress();
        auto writtenBytesCount =
                sendto(
                        getNativeHandler(),
                        buf,
                        size,
                        so.getAsIntegral(),
                        reinterpret_cast<const sockaddr *>(&addr),
                        sizeof(addr)
                );
        if (writtenBytesCount == -1) {
            return fromErrno<WriteCount>();
        }

        return writtenBytesCount;
    }

    ReadCountRet
    UdpClientBase::read(void *extBuffer, std::size_t size, const ReadOption &ro) const noexcept {
        const auto recBytes =
                recvfrom(
                        getNativeHandler(),
                        extBuffer,
                        size,
                        ro.getAsIntegral(),
                        nullptr,
                        nullptr);

        if (recBytes == -1) {
            return fromErrno<ReadCount>();
        }

        return recBytes;
    }
}