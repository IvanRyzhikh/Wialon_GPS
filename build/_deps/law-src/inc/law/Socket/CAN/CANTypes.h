#pragma once

#include <span>
#include <variant>
#include <linux/can.h>

#include "law/Types.h"

namespace law::socket::can {
    namespace detail {
        using CANAddress = sockaddr_can;
        using CANAddressRet = CustomError<CANAddress>;
    }

    enum class CANMode {
        CAN = CAN_MTU,
        CAN_FD = CANFD_MTU,
#ifdef CANXL_MTU
        CAN_XL = CANXL_MTU,
#endif
    };

    enum class CANFlags : std::uint32_t {
        EFF = CAN_EFF_FLAG,
        RTR = CAN_RTR_FLAG,
        ERR = CAN_ERR_FLAG
    };

    using CANFrame = std::variant<
            can_frame,
            canfd_frame
#ifdef CANXL_MTU
            , canxl_frame
#endif
            >;
    using CANFrameRet = CustomError<CANFrame>;

    using CANFrameSequence = std::span<const CANFrame>;

    using CANFilter = can_filter;
    using CANFilterSequence = std::span<const CANFilter>;
}
