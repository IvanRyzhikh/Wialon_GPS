#pragma once

#include "BCMOpcode.h"
#include "law/Socket/CAN/CANTypes.h"
#include "law/Socket/CAN/Base/CANBase.h"

namespace law::socket::can {
    class BCM : public detail::CANBase {
    public:
        BCM() noexcept = default;

        explicit BCM(std::string_view canIfName) noexcept;

        Status reattach(std::string_view canIfName) noexcept;

        Status reattach() noexcept;

        Status attach(std::string_view canIfName) noexcept;

        Status detach() noexcept;

        WriteCountRet write(const CANFrame &frame) noexcept;

        WriteCountRet write(const CANFrame &frame, std::string_view canIfName) noexcept;

        WriteCountRet write(const void *buffer, std::size_t size) noexcept;

        WriteCountRet write(const void *buffer, std::size_t size, std::string_view canIfNam) noexcept;

        ReadCountRet read(void *buffer, std::size_t bufferSize) noexcept;

    private:
        Status connect(const sockaddr_can &addr) noexcept;
    };
}
