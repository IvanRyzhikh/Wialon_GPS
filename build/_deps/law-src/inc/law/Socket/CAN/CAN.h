#pragma once

#include "CANTypes.h"
#include "law/Socket/CAN/Base/CANBase.h"

namespace law::socket::can {
    class CAN final : public detail::CANBase {
    public:
        CAN() noexcept = default;

        CAN(std::string_view canIfName, CANMode mode = CANMode::CAN);

        Status reattach() noexcept;

        Status reattach(std::string_view canIfName, CANMode mode = CANMode::CAN) noexcept;

        Status attach(std::string_view canIfName, CANMode mode = CANMode::CAN) noexcept;

        Status detach() noexcept;

        [[nodiscard]] Status write(const CANFrameSequence &frames) const noexcept;

        [[nodiscard]] Status write(const CANFrame &frame) const noexcept;

        [[nodiscard]] CANFrameRet read() const noexcept;

        [[nodiscard]] Status applyFilter(CANFilter filter) const noexcept;

        [[nodiscard]] Status applyFilter(CANFilterSequence filters) const noexcept;

        [[nodiscard]] Status dropAllFilters() const noexcept;

    private:
        Status bind(const sockaddr_can &addr, CANMode mode) noexcept;
    };
}
