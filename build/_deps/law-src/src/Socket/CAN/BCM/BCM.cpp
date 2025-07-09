#include <cstring>
#include <unistd.h>
#include <linux/can/bcm.h>

#include <errcode/errcode.h>

#include <law/Socket/CAN/BCM/BCM.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wgnu-variable-sized-type-not-at-end"
struct BCMMessage {
    bcm_msg_head msgHead;
    canfd_frame frame;
};
#pragma GCC diagnostic pop

namespace {
    template <typename T>
    BCMMessage makeGenericMessage(const T &frame, std::uint32_t opcodes) noexcept {
        BCMMessage msg;
        std::memset(&msg, 0, sizeof(BCMMessage));

        msg.msgHead.opcode = opcodes;
        msg.msgHead.can_id = frame.can_id;
        msg.msgHead.nframes = 1;

        if constexpr (std::same_as<canfd_frame, T>) {
            msg.msgHead.flags = CAN_FD_FRAME;
            msg.frame = frame;
        } else {
            std::memcpy(&msg.frame, &frame, sizeof(frame));
        }

        return msg;
    }

    sockaddr_can makeAddress(std::uint32_t ifIndex) noexcept {
        sockaddr_can addr;

        addr.can_family = AF_CAN;
        addr.can_ifindex = ifIndex;

        return addr;
    }
}

namespace law::socket::can {
    static WriteCountRet genericWrite(int fd, const BCMMessage &msg) noexcept {
        const auto wc = ::write(fd, &msg, sizeof(msg));
        if (wc == -1) {
            return fromErrno<WriteCount>();
        }

        return wc;
    }

    static WriteCountRet genericWrite(int fd, const BCMMessage &msg, const sockaddr_can &addr) noexcept {
        const auto wc = ::sendto(
            fd,
            &msg,
            sizeof(msg),
            0,
            reinterpret_cast<const sockaddr *>(&addr),
            sizeof(addr)
        );
        if (wc == -1) {
            return fromErrno<WriteCount>();
        }

        return wc;
    }

    BCM::BCM(std::string_view canIfName) noexcept {
        if (!attach(canIfName)) {
            throwErrno();
        }
    }

    Status BCM::reattach(std::string_view canIfName) noexcept {
        EXPECTED_CHECK(detach());
        return attach(canIfName);
    }

    Status BCM::reattach() noexcept {
        EXPECTED_AUTO(addr, obtainCurrentAddress());
        EXPECTED_CHECK(detach());
        return connect(addr);
    }

    Status BCM::attach(std::string_view canIfName) noexcept {
        EXPECTED_CHECK(makeSocket(socket::detail::SocketType::DGRAM, CAN_BCM));

        ifreq ifr;
        EXPECTED_AUTO(ifIndex, obtainCanIfIndex(ifr, canIfName));

        return connect(makeAddress(ifIndex));
    }

    Status BCM::detach() noexcept {
        return close();
    }

    WriteCountRet BCM::write(const CANFrame &frame) noexcept {
        return std::visit(
            [this]<typename T>(const T &fr) noexcept -> WriteCountRet {
#ifdef CANXL_MTU
                if constexpr (std::same_as<canxl_frame, T>) {
                    return std::unexpected(RetCode::NOT_SUPPORTED);
                } else {
#endif
                    const auto msg = makeGenericMessage(fr, TX_SEND);
                    return genericWrite(getNativeHandler(), msg);
#ifdef CANXL_MTU
                }
#endif
            },
            frame
        );
    }

    WriteCountRet BCM::write(const CANFrame &frame, std::string_view canIfName) noexcept {
        return std::visit(
            [this, canIfName]<typename T>(const T &fr) noexcept -> WriteCountRet {
#ifdef CANXL_MTU
                if constexpr (std::same_as<canxl_frame, T>) {
                    return std::unexpected(RetCode::NOT_SUPPORTED);
                } else {
#endif
                    ifreq ifr;
                    EXPECTED_AUTO(ifIndex, obtainCanIfIndex(ifr, canIfName));
                    const auto msg = makeGenericMessage(fr, TX_SEND);
                    return genericWrite(getNativeHandler(), msg, makeAddress(ifIndex));
#ifdef CANXL_MTU
                }
#endif
            },
            frame
        );
    }

    WriteCountRet BCM::write(const void *buffer, std::size_t size) noexcept {
        const auto wc = ::write(getNativeHandler(), buffer, size);
        if (wc == -1) {
            return fromErrno<WriteCount>();
        }

        return wc;
    }

    WriteCountRet BCM::write(const void *buffer, std::size_t size, std::string_view canIfName) noexcept {
        ifreq ifr;
        EXPECTED_AUTO(ifIndex, obtainCanIfIndex(ifr, canIfName));

        const auto addr = makeAddress(ifIndex);
        const auto wc = ::sendto(
            getNativeHandler(),
            buffer,
            size,
            0,
            reinterpret_cast<const sockaddr *>(&addr),
            sizeof(addr)
        );
        if (wc == -1) {
            return fromErrno<WriteCount>();
        }

        return wc;
    }

    ReadCountRet BCM::read(void *buffer, std::size_t bufferSize) noexcept {
        const auto rc = ::read(getNativeHandler(), buffer, bufferSize);
        if (rc == -1) {
            return fromErrno<ReadCount>();
        }

        return rc;
    }

    Status BCM::connect(const sockaddr_can &addr) noexcept {
        return LAW_INTERNAL_ERROR_CHECK(
            ::connect(getNativeHandler(), reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)));
    }
}
