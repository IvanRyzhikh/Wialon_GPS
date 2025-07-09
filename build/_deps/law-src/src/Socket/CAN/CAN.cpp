#include <array>
#include <cstring>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/can/raw.h>

#include <boost/assert.hpp>
#include <errcode/errcode.h>

#include "law/Types.h"
#include "law/Utils.h"
#include "law/Socket/CAN/CAN.h"
#include "law/RetCode/RetCode.h"

namespace {
    std::uint8_t canDlc2Len(std::uint8_t dlc) noexcept {
        static constexpr std::array<std::uint8_t, 16> kDlc2LenTable = {
            0, 1, 2, 3, 4, 5, 6, 7,
            8, 12, 16, 20, 24, 32, 48, 64
        };
        return kDlc2LenTable[dlc & 0x0F];
    }

    std::uint8_t canLen2Dlc(std::uint8_t len) noexcept {
        static constexpr std::array<std::uint8_t, 65> kLen2DlcTable = {
            0, 1, 2, 3, 4, 5, 6, 7, 8,
            9, 9, 9, 9,
            10, 10, 10, 10,
            11, 11, 11, 11,
            12, 12, 12, 12,
            13, 13, 13, 13, 13, 13, 13, 13,
            14, 14, 14, 14, 14, 14, 14, 14,
            14, 14, 14, 14, 14, 14, 14, 14,
            15, 15, 15, 15, 15, 15, 15, 15,
            15, 15, 15, 15, 15, 15, 15, 15
        };
        return ((len <= 64) ? kLen2DlcTable[len] : 0x0F);
    }
}

namespace law::socket::can {
    CAN::CAN(std::string_view canIfName, CANMode mode) {
        if (!attach(canIfName, mode)) {
            throwErrno();
        }
    }

    Status CAN::reattach() noexcept {
        EXPECTED_AUTO(addr, obtainCurrentAddress());

        ifreq ifr;
        if (ioctl(getNativeHandler(), SIOCGIFMTU, &ifr) == -1) {
            return fromErrno();
        }

        EXPECTED_CHECK(detach());
        return bind(addr, static_cast<CANMode>(ifr.ifr_mtu));
    }

    Status CAN::reattach(std::string_view canIfName, CANMode mode) noexcept {
        EXPECTED_CHECK(detach());
        return attach(canIfName, mode);
    }

    Status CAN::attach(std::string_view canIfName, CANMode mode) noexcept {
        using namespace law::socket::detail;
        EXPECTED_CHECK(makeSocket(SocketType::RAW, CAN_RAW));

        ifreq ifr;
        EXPECTED_AUTO(ifIndex, obtainCanIfIndex(ifr, canIfName));

        if (ioctl(getNativeHandler(), SIOCGIFMTU, &ifr) == -1) {
            return fromErrno();
        }

        if (std::to_underlying(mode) > ifr.ifr_mtu) {
            return fromLawError(RetCode::NOT_SUPPORTED);
        }

        sockaddr_can addr;
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifIndex;

        return bind(addr, mode);
    }

    Status CAN::detach() noexcept {
        return close();
    }

    Status CAN::write(const CANFrameSequence &frames) const noexcept {
        for (const auto &fr: frames) {
            EXPECTED_CHECK(write(fr));
        }

        return {};
    }

    Status CAN::write(const CANFrame &frame) const noexcept {
        return std::visit(
            [this]<typename T>(const T &fr) {
                if constexpr (std::same_as<canfd_frame, T>) {
                    auto canFrame = fr;
                    canFrame.len = canDlc2Len(canLen2Dlc(fr.len));
                    return LAW_INTERNAL_ERROR_CHECK(::write(getNativeHandler(), &canFrame, CANFD_MTU));
                } else if constexpr (std::same_as<can_frame, T>) {
                    return LAW_INTERNAL_ERROR_CHECK(::write(getNativeHandler(), &fr, CAN_MTU));
                }
#ifdef CANXL_MTU
                else {
                    return LAW_INTERNAL_ERROR_CHECK(::write(getNativeHandler(), &fr, CANXL_MTU));
                }
#endif
            },
            frame
        );
    }

    [[nodiscard]] CANFrameRet CAN::read() const noexcept {
#ifdef CANXL_MTU
        std::array<std::uint8_t, CANXL_MTU> data;
#else
        std::array<std::uint8_t, CANFD_MTU> data;
#endif

        const auto recBytes = ::read(getNativeHandler(), data.data(), data.size());
        if (recBytes == -1) {
            return fromErrno<CANFrame>();
        }

        switch (recBytes) {
            case CAN_MTU: {
                return *(reinterpret_cast<const can_frame *>(data.data()));
            }
            case CANFD_MTU: {
                return *(reinterpret_cast<const canfd_frame *>(data.data()));
            }
#ifdef CANXL_MTU
            case CANXL_MTU: {
                return *(reinterpret_cast<const canxl_frame *>(data.data()));
            }
#endif
            default: {
                __builtin_unreachable();
                return can_frame{};
            }
        }
    }

    Status CAN::applyFilter(CANFilter filter) const noexcept {
        return applyFilter({&filter, 1});
    }

    Status CAN::applyFilter(CANFilterSequence filterSequence) const noexcept {
        return applyOption(
            SocketOptionLevel::RAW_CAN, SocketOption::CANRAW_FILTER,
            filterSequence.data(), (filterSequence.size() * sizeof(CANFilter))
        );
    }

    Status CAN::dropAllFilters() const noexcept {
        const int *dummyPtr = nullptr;
        return applyOption(SocketOptionLevel::RAW_CAN, SocketOption::CANRAW_FILTER, dummyPtr, 0);
    }

    Status CAN::bind(const sockaddr_can &addr, CANMode mode) noexcept {
        if (::bind(getNativeHandler(), reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)) == -1) {
            return fromErrno();
        }

        switch (mode) {
            case CANMode::CAN_FD: {
                EXPECTED_CHECK(
                    applyOption(
                        SocketOptionLevel::RAW_CAN, CAN_RAW_FD_FRAMES, &utils::detail::kOptionEnableVal
                    )
                );
                return {};
            }
#ifdef CANXL_MTU
            case CANMode::CAN_XL: {
                EXPECTED_CHECK(
                    applyOption(
                        SocketOptionLevel::RAW_CAN, CAN_RAW_XL_FRAMES, &utils::detail::kOptionEnableVal
                    )
                );
                return {};
            }
#endif
            default: {
                return {};
            }
        }
    }
}
