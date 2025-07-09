#pragma once

#include <law/Types.h>

#include "Mode.h"

namespace law::spi {
    using ModeRet = CustomError<Mode>;

    enum BitMode : std::uint8_t {
        MSB = 0,
        LSB = 1
    };
    using BitModeRet = CustomError<BitMode>;

    using BitsPerWord = std::uint8_t;
    using BitsPerWordRet = CustomError<BitsPerWord>;

    using SpeedHz = std::uint32_t;
    using SpeedHzRet = CustomError<SpeedHz>;
}
