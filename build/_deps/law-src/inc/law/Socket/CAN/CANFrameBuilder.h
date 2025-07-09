#pragma once

#include "CANTypes.h"
#include "CANFlags.h"

namespace law::socket::can {
    class CANFrameBuilder {
    private:
        class FrameBuilder final {
        private:
            can_frame &rFrame;

        public:
            FrameBuilder(can_frame &frameRef) noexcept;

            FrameBuilder &setId(canid_t id) noexcept;

            FrameBuilder &setId(canid_t id, CANFlag flags) noexcept;

            FrameBuilder &setData(std::string_view data) noexcept;

            FrameBuilder &setData(const void *data, std::size_t size) noexcept;

            CANFrame get() noexcept;
        };

        class FDFrameBuilder final {
        private:
            canfd_frame &rFrame;

        public:
            FDFrameBuilder(canfd_frame &frameRef) noexcept;

            FDFrameBuilder &setId(canid_t id) noexcept;

            FDFrameBuilder &setId(canid_t id, CANFlag flags) noexcept;

            FDFrameBuilder &setFlags(CANFDFlag flags) noexcept;

            FDFrameBuilder &setData(std::string_view data) noexcept;

            FDFrameBuilder &setData(const void *data, std::size_t size) noexcept;

            CANFrame get() noexcept;
        };

#ifdef CANXL_MTU
        class XLFrameBuilder final {
        private:
            canxl_frame &rFrame;

        public:
            XLFrameBuilder(canxl_frame &frameRef) noexcept;

            XLFrameBuilder &setId(std::uint8_t vcid, std::uint16_t prio) noexcept;

            XLFrameBuilder &setFlags(CANXLFlag flags) noexcept;

            XLFrameBuilder &setSDU(std::uint8_t sdu) noexcept;

            XLFrameBuilder &setAF(std::uint32_t af) noexcept;

            XLFrameBuilder &setData(std::string_view data) noexcept;

            XLFrameBuilder &setData(const void *data, std::size_t size) noexcept;

            CANFrame get() noexcept;
        };
#endif

    private:
        CANFrame frame;

    public:
        FrameBuilder createFrame() noexcept;

        FDFrameBuilder createFdFrame() noexcept;

#ifdef CANXL_MTU
        XLFrameBuilder createXLFrame() noexcept;
#endif
    };
}
