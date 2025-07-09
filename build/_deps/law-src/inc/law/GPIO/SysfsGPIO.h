#pragma once

#include <boost/assert.hpp>

#include "Edge.h"
#include "Port.h"
#include "PinMode.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::gpio {
    class SysfsGPIO final : public fd::FileDescriptor {
    private:
        using Base = FileDescriptor;

        using Base::applyOption;
        using Base::readOption;

    private:
        std::uint16_t sysfsPinNum = 0xFFFF;

    public:
        SysfsGPIO() noexcept = default;

        SysfsGPIO(Port gpioPort, std::uint8_t pinNumber);

        SysfsGPIO(std::uint16_t sysfsPinNumber);

        Status setPinMode(PinMode mode) noexcept;

        Status setPin(Port port, std::uint8_t pinNumber) noexcept;

        Status setPin(std::uint16_t sysfsPinNumber) noexcept;

        Status setEdge(Edge edge) noexcept;

        Status set() noexcept;

        Status reset() noexcept;

        Status setState(bool status) noexcept;

        BoolRet get() noexcept;

    private:
        Status resetValueFilePos() noexcept;
    };
}
