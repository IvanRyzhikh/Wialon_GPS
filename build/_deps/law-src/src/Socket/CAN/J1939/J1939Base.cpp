#include <cstring>
#include <unistd.h>
#include <sys/uio.h>
#include <linux/can.h>

#include <errcode/errcode.h>

#include "law/Utils.h"
#include "law/Socket/CAN/J1939/J1939Base.h"

namespace {
    sockaddr_can makeCanAddr(int ifIndex, pgn_t pgn) noexcept {
        return {
            .can_family = AF_CAN,
            .can_ifindex = ifIndex,
            .can_addr = {
                .j1939 = {
                    .name = J1939_NO_NAME,
                    .pgn = pgn,
                    .addr = J1939_NO_ADDR
                }
            }
        };
    }
}

namespace law::socket::can::detail {
    J1939Base::J1939Base(
        std::string_view canIfName,
        std::uint8_t address,
        std::uint32_t pgn,
        std::uint64_t name
    ) {
        if (!attach(canIfName, address, pgn, name)) {
            throwErrno();
        }
    }

    Status J1939Base::reattach() noexcept {
        sockaddr_can addr;
        socklen_t len = sizeof(addr);

        if (getsockname(getNativeHandler(), reinterpret_cast<sockaddr *>(&addr), &len) == -1) {
            return fromErrno();
        }

        ifreq ifr;
        if (ioctl(getNativeHandler(), SIOCGIFMTU, &ifr) == -1) {
            return fromErrno();
        }

        EXPECTED_CHECK(detach());
        return bind(addr);
    }

    Status J1939Base::reattach(
        std::string_view canIfName,
        std::uint8_t address,
        std::uint32_t pgn,
        std::uint64_t name
    ) noexcept {
        EXPECTED_CHECK(detach());
        return attach(canIfName, address, pgn, name);
    }

    Status J1939Base::attach(
        std::string_view canIfName,
        std::uint8_t address,
        std::uint32_t pgn,
        std::uint64_t name
    ) noexcept {
        using namespace law::socket::detail;
        EXPECTED_CHECK(makeSocket(socket::detail::SocketDomain::CAN, SocketType::DGRAM, CAN_J1939));

        ifreq ifr;

        static constexpr int kAllCanIfaceIndex = 0;
        ifr.ifr_ifindex = kAllCanIfaceIndex;

        if (!canIfName.empty()) {
            std::strncpy(ifr.ifr_name, canIfName.data(), (IFNAMSIZ - 1));
            if (ioctl(getNativeHandler(), SIOCGIFINDEX, &ifr) == -1) {
                return fromErrno();
            }
        }

        const sockaddr_can addr = {
            .can_family = AF_CAN,
            .can_ifindex = ifr.ifr_ifindex,
            .can_addr = {
                .j1939 = {
                    .name = name,
                    .pgn = pgn,
                    .addr = address
                }
            }
        };

        return bind(addr);
    }

    Status J1939Base::detach() noexcept {
        return close();
    }

    Status J1939Base::connect(std::uint8_t addr, pgn_t pgn) const noexcept {
        sockaddr_can sockAddr;
        socklen_t len = sizeof(sockAddr);

        if (getsockname(getNativeHandler(), reinterpret_cast<sockaddr *>(&sockAddr), &len) == -1) {
            return fromErrno();
        }

        const sockaddr_can destAddr = {
            .can_family = AF_CAN,
            .can_ifindex = sockAddr.can_ifindex,
            .can_addr = {
                .j1939 = {
                    .name = J1939_NO_NAME,
                    .pgn = pgn,
                    .addr = addr
                }
            }
        };
        return LAW_INTERNAL_ERROR_CHECK(
            ::connect(
                getNativeHandler(),
                reinterpret_cast<const sockaddr *>(&destAddr),
                sizeof(sockaddr_can)
            )
        );
    }

    Status J1939Base::applyFilter(const J1939Filter &filter) const noexcept {
        J1939Filter arr[1];
        arr[0] = filter;
        return applyFilter(arr);
    }

    Status J1939Base::applyFilter(J1939FilterSequence filterSequence) const noexcept {
        return applyOption(
            SocketOptionLevel::J1939, SocketOption::J1939_FILTER,
            filterSequence.data(), (filterSequence.size() * sizeof(J1939Filter))
        );
    }

    Status J1939Base::setSendPriority(J1939SendPriority priority) const noexcept {
        const auto prio = static_cast<int>(priority);
        return applyOption(SocketOptionLevel::J1939, SocketOption::J1939_SEND_PRIO, &prio);
    }

    J1939SendPriorityRet J1939Base::getSendPriority() const noexcept {
        int prio = {};
        EXPECTED_CHECK(readOption(SocketOptionLevel::J1939, SocketOption::J1939_SEND_PRIO, &prio));
        return static_cast<J1939SendPriority>(prio);
    }

    Status J1939Base::broadcast(bool status) const noexcept {
        return applyOption(
            SocketOptionLevel::SOCKET,
            SocketOption::BROADCAST,
            (status ? &utils::detail::kOptionEnableVal : &utils::detail::kOptionDisableVal)
        );
    }

    BoolRet J1939Base::broadcast() const noexcept {
        int state = {};
        EXPECTED_CHECK(readOption(SocketOptionLevel::SOCKET, SocketOption::BROADCAST, &state));
        return state;
    }

    Status J1939Base::promiscuous(bool status) const noexcept {
        return applyOption(
            SocketOptionLevel::J1939,
            SocketOption::J1939_PROMISC,
            (status ? &utils::detail::kOptionEnableVal : &utils::detail::kOptionDisableVal)
        );
    }

    BoolRet J1939Base::promiscuous() const noexcept {
        int state = {};
        EXPECTED_CHECK(readOption(SocketOptionLevel::J1939, SocketOption::J1939_PROMISC, &state));
        return state;
    }

    WriteCountRet J1939Base::write(const void *extBuffer, std::size_t size) const noexcept {
        return write({static_cast<const char *>(extBuffer), size});
    }

    WriteCountRet J1939Base::write(const void *extBuffer, std::size_t size, pgn_t dest) const noexcept {
        return write({static_cast<const char *>(extBuffer), size}, dest);
    }

    WriteCountRet J1939Base::write(std::string_view data) const noexcept {
        const auto bytesWrittenCount =
                ::write(getNativeHandler(), data.data(), data.size());
        if (bytesWrittenCount == -1) {
            return fromErrno<WriteCount>();
        }

        return bytesWrittenCount;
    }

    WriteCountRet J1939Base::write(std::string_view data, pgn_t dest) const noexcept {
        const auto destAddr = makeCanAddr(ifIndex, dest);
        const auto bytesWrittenCount =
                sendto(
                    getNativeHandler(),
                    data.data(),
                    data.size(),
                    0,
                    reinterpret_cast<const sockaddr *>(&destAddr),
                    sizeof(sockaddr_can)
                );
        if (bytesWrittenCount == -1) {
            return fromErrno<WriteCount>();
        }

        return bytesWrittenCount;
    }

    WriteCountRet J1939Base::writev(IoVectorSet set) noexcept {
        const auto bytesWrittenCount =
                ::writev(getNativeHandler(), set.data(), static_cast<int>(set.size()));
        if (bytesWrittenCount == -1) {
            return fromErrno<WriteCount>();
        }

        return bytesWrittenCount;
    }

    WriteCountRet J1939Base::writev(IoVectorSet set, pgn_t dest) noexcept {
        msghdr msg;
        std::memset(&msg, 0, sizeof(msghdr));

        const auto destAddr = makeCanAddr(ifIndex, dest);

        msg.msg_name = const_cast<sockaddr_can *>(&destAddr);
        msg.msg_namelen = sizeof(destAddr);
        msg.msg_iov = const_cast<iovec *>(set.data());
        msg.msg_iovlen = set.size();

        const auto bytesWrittenCount = sendmsg(getNativeHandler(), &msg, 0);
        if (bytesWrittenCount == -1) {
            return fromErrno<WriteCount>();
        }

        return bytesWrittenCount;
    }

    ReadCountRet J1939Base::read(void *extBuffer, std::size_t readCount) const noexcept {
        const auto recBytes = ::read(getNativeHandler(), extBuffer, readCount);

        if (recBytes == -1) {
            return fromErrno<ReadCount>();
        }

        if (recBytes == 0) {
            return fromLawError<ReadCount>(RetCode::READ_EMPTY_DATA);
        }

        return recBytes;
    }

    ReadResultRet J1939Base::readFrom(void *extBuffer, std::size_t readCount) const noexcept {
        sockaddr_can recAddress{};
        socklen_t addressLength = sizeof(sockaddr_can);

        const auto recBytes =
                ::recvfrom(
                    getNativeHandler(),
                    extBuffer,
                    readCount,
                    0,
                    reinterpret_cast<sockaddr *>(&recAddress),
                    &addressLength
                );

        if (recBytes == -1) {
            return fromErrno<ReadResult>();
        }

        if (recBytes == 0) {
            return fromLawError<ReadResult>(RetCode::READ_EMPTY_DATA);
        }

        return ReadResult{recAddress, std::size_t(recBytes)};
    }

    Status J1939Base::bind(const sockaddr_can &addr) const noexcept {
        return LAW_INTERNAL_ERROR_CHECK(
            ::bind(getNativeHandler(), reinterpret_cast<const sockaddr *>(&addr), sizeof(addr))
        );
    }
}
