#include <linux/can.h>

#include "law/Socket/CAN/CANFlags.h"

namespace law::socket::can {
    CANFlag CANFlag::operator|(const CANFlag &cf) const noexcept {
        return CANFlag{getAsIntegral() | cf.getAsIntegral()};
    }

    EFF::EFF() noexcept: CANFlag(CAN_EFF_FLAG) {}

    RTR::RTR() noexcept: CANFlag(CAN_RTR_FLAG) {}

    ERR::ERR() noexcept: CANFlag(CAN_ERR_FLAG) {}

    CANFDFlag CANFDFlag::operator|(const CANFDFlag &cf) const noexcept {
        return CANFDFlag{getAsIntegral() | cf.getAsIntegral()};
    }

    BRS::BRS() noexcept : CANFDFlag(CANFD_BRS) {}

    ESI::ESI() noexcept : CANFDFlag(CANFD_ESI) {}

#ifdef CANFD_FDF
    FDF::FDF() noexcept: CANFDFlag(CANFD_FDF) {}
#endif

#ifdef CANXL_MTU
    CANXLFlag CANXLFlag::operator|(const CANXLFlag &cf) const noexcept {
        return CANXLFlag{getAsIntegral() | cf.getAsIntegral()};
    }

    XLF::XLF() noexcept: CANXLFlag(CANXL_XLF) {}

    SEC::SEC() noexcept: CANXLFlag(CANXL_SEC) {}
#endif
}
