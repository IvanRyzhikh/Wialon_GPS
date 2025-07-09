#include <unistd.h>
#include <charconv>

#include "law/Utils.h"
#include "law/Socket/Unix/UDP/Base/UdsUdpSocketBase.h"

namespace {
    auto makeSockPathCommonPart() noexcept {
        static constexpr char kPathCommonPath[] = "/tmp/.law-private-uds-udp-sock-client.";
        static constexpr std::uint8_t kMaxPidLength = 5;
        static constexpr std::uint8_t kPathLength = sizeof(kPathCommonPath) + kMaxPidLength + sizeof(std::uint16_t) + 1;

        static_assert(kPathLength, "Sockpath should get into sockaddr_un");

        etl::string<kPathLength> path = kPathCommonPath;

        path.resize(kPathLength);
        auto [ptr, ec] = std::to_chars(&path[sizeof(kPathCommonPath) - 1], (path.data() + path.capacity()), getpid());
        path.resize(ptr - path.data());
        path += '.';

        return path;
    }
}

namespace law::socket::uds::udp::detail {
    WriteCountRet
    UdsUdpSocketBase::write(std::string_view data, const UnixAddress &addr, const SendOption &so) const noexcept {
        return write(data.data(), data.size(), addr, so);
    }

    [[nodiscard]] WriteCountRet UdsUdpSocketBase::write(const msghdr &message, const UnixAddress &dest) const noexcept {
        auto addr = dest.getNativeAddress();

        auto msg = message;
        msg.msg_name = reinterpret_cast<void *>(&addr);
        msg.msg_namelen = sizeof(sockaddr_un);

        auto rc = sendmsg(getNativeHandler(), &msg, 0);
        if (rc == -1) {
            return fromErrno<WriteCount>();
        }

        return rc;
    }

    WriteCountRet
    UdsUdpSocketBase::write(const void *buf, std::size_t size, const UnixAddress &addr, const SendOption &so) const noexcept {
        auto writtenBytesCount =
                sendto(
                        getNativeHandler(),
                        buf,
                        size,
                        so.getAsIntegral(),
                        reinterpret_cast<const sockaddr *>(&addr.getNativeAddress()),
                        sizeof(sockaddr_un)
                );
        if (writtenBytesCount == -1) {
            return fromErrno<WriteCount>();
        }

        return writtenBytesCount;
    }

    Status UdsUdpSocketBase::makeSocket() noexcept {
        return uds::detail::UnixSocketBase::makeSocket(socket::detail::SocketType::DGRAM);
    }

    UdsUdpSocketBase::UnixPath UdsUdpSocketBase::makeRandomUnixPath() noexcept {
        auto path = makeSockPathCommonPart();

        static std::uint16_t partNumber = 0;

        const auto currentEnd = path.end();
        path.resize(path.capacity());
        auto [ptr, ec] = std::to_chars(currentEnd, (path.data() + path.capacity()), partNumber++);
        path.resize(ptr - path.data());

        return path;
    }
}
