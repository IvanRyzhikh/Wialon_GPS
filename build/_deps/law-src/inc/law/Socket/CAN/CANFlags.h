#pragma once

#include "law/FlagValue.h"

namespace law::socket::can {
    struct CANFlag : public UInt32FlagValue {
        using UInt32FlagValue::UInt32FlagValue;

        CANFlag operator|(const CANFlag &cf) const noexcept;
    };

    struct EFF : public CANFlag {
        EFF() noexcept;
    };

    struct RTR : public CANFlag {
        RTR() noexcept;
    };

    struct ERR : public CANFlag {
        ERR() noexcept;
    };

    struct CANFDFlag : public UInt32FlagValue {
        using UInt32FlagValue::UInt32FlagValue;

        CANFDFlag operator|(const CANFDFlag &cf) const noexcept;
    };

    struct BRS : public CANFDFlag {
        BRS() noexcept;
    };

    struct ESI : public CANFDFlag {
        ESI() noexcept;
    };

#ifdef CANFD_FDF
    struct FDF : public CANFDFlag {
        FDF() noexcept;
    };
#endif

#ifdef CANXL_MTU
    struct CANXLFlag : public UInt32FlagValue {
        using UInt32FlagValue::UInt32FlagValue;

        CANXLFlag operator|(const CANXLFlag &cf) const noexcept;
    };

    struct XLF : public CANXLFlag {
        XLF() noexcept;
    };

    struct SEC : public CANXLFlag {
        SEC() noexcept;
    };
#endif
}
