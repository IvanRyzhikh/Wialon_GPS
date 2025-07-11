#pragma once

#define termios asmtermios
#include <asm-generic/termbits.h>
#include <asm-generic/ioctls.h>
#undef termios
#include <cstdint>

#include "law/RetCode/RetCode.h"
#include "law/Types.h"

namespace law::uart {
    enum class DataBits : std::uint8_t {
        BITS_5 = 5,
        BITS_6 = 6,
        BITS_7 = 7,
        BITS_8 = 8
    };

    enum class Parity : char {
        NONE = 'n',
        EVEN = 'e',
        ODD = 'o'
    };

    enum class StopBits : std::uint8_t {
        BITS_1 = 1,
        BITS_2 = 2
    };

    enum class FlowControl : bool {
        ENABLE = true,
        DISABLE = false
    };

    class UartConfiguration {
    private:
        termios2 term{};

    public:
        UartConfiguration() noexcept;

        UartConfiguration &setDataBits(DataBits dataBits) noexcept;

        UartConfiguration &setParity(Parity parity) noexcept;

        UartConfiguration &setStopBits(StopBits stopBits) noexcept;

        UartConfiguration &setFlowControl(FlowControl flowControl) noexcept;

        UartConfiguration &setBaudRate(std::uint32_t baudrate) noexcept;

        [[nodiscard]] DataBits getDataBits() const noexcept;

        [[nodiscard]] Parity getParity() const noexcept;

        [[nodiscard]] StopBits getStopBits(StopBits) const noexcept;

        [[nodiscard]] FlowControl getFlowControl(FlowControl) const noexcept;

        [[nodiscard]] std::uint32_t getBaudrate() const noexcept;

        [[nodiscard]] const termios2 &getConfiguration() const noexcept;
    };
}