#include <cstring>

#include <law/Socket/CAN/CANFrameBuilder.h>

namespace law::socket::can {
    CANFrameBuilder::FrameBuilder::FrameBuilder(can_frame &frameRef) noexcept: rFrame(frameRef) {
    }

    CANFrameBuilder::FrameBuilder &CANFrameBuilder::FrameBuilder::setId(canid_t id) noexcept {
        rFrame.can_id = id;
        if (id & CAN_ERR_FLAG) {
            rFrame.can_id |= CAN_EFF_FLAG;
        }

        return *this;
    }

    CANFrameBuilder::FrameBuilder &CANFrameBuilder::FrameBuilder::setId(canid_t id, CANFlag flags) noexcept {
        rFrame.can_id = (id | flags.getAsIntegral());
        return *this;
    }

    CANFrameBuilder::FrameBuilder &CANFrameBuilder::FrameBuilder::setData(std::string_view data) noexcept {
        return setData(data.data(), data.size());
    }

    CANFrameBuilder::FrameBuilder &CANFrameBuilder::FrameBuilder::setData(const void *data, std::size_t size) noexcept {
        const auto len = ((size > CAN_MAX_DLEN) ? CAN_MAX_DLEN : size);

        rFrame.can_dlc = len;
        std::memcpy(rFrame.data, data, len);

        return *this;
    }

    CANFrame CANFrameBuilder::FrameBuilder::get() noexcept {
        return rFrame;
    }

    CANFrameBuilder::FDFrameBuilder::FDFrameBuilder(canfd_frame &frameRef) noexcept: rFrame(frameRef) {
#ifdef CANFD_FDF
        rFrame.flags = CANFD_FDF;
#endif
    }

    CANFrameBuilder::FDFrameBuilder &CANFrameBuilder::FDFrameBuilder::setId(canid_t id) noexcept {
        rFrame.can_id = id;
        if (id & CAN_ERR_FLAG) {
            rFrame.can_id |= CAN_EFF_FLAG;
        }

        return *this;
    }

    CANFrameBuilder::FDFrameBuilder &CANFrameBuilder::FDFrameBuilder::setId(canid_t id, CANFlag flags) noexcept {
        rFrame.can_id = (id | flags.getAsIntegral());
        return *this;
    }

    CANFrameBuilder::FDFrameBuilder &CANFrameBuilder::FDFrameBuilder::setFlags(CANFDFlag flags) noexcept {
        rFrame.flags = flags.getAsIntegral();
        return *this;
    }

    CANFrameBuilder::FDFrameBuilder &CANFrameBuilder::FDFrameBuilder::setData(std::string_view data) noexcept {
        return setData(data.data(), data.size());
    }

    CANFrameBuilder::FDFrameBuilder &CANFrameBuilder::FDFrameBuilder::setData(const void *data, std::size_t size) noexcept {
        const auto len = ((size > CANFD_MAX_DLEN) ? CANFD_MAX_DLEN : size);

        rFrame.len = len;
        std::memcpy(rFrame.data, data, len);

        return *this;
    }

    CANFrame CANFrameBuilder::FDFrameBuilder::get() noexcept {
        return rFrame;
    }

#ifdef CANXL_MTU
    CANFrameBuilder::XLFrameBuilder::XLFrameBuilder(canxl_frame &frameRef) noexcept: rFrame(frameRef) {
        rFrame.flags = CANXL_XLF;
    }

    CANFrameBuilder::XLFrameBuilder &CANFrameBuilder::XLFrameBuilder::setId(std::uint8_t vcid, std::uint16_t prio) noexcept {
        rFrame.prio = ((static_cast<std::uint32_t>(vcid) << CANXL_VCID_OFFSET) | prio);
        return *this;
    }

    CANFrameBuilder::XLFrameBuilder &CANFrameBuilder::XLFrameBuilder::setFlags(CANXLFlag flags) noexcept {
        rFrame.flags |= flags.getAsIntegral();
        return *this;
    }

    CANFrameBuilder::XLFrameBuilder &CANFrameBuilder::XLFrameBuilder::setSDU(std::uint8_t sdu) noexcept {
        rFrame.sdt = sdu;
        return *this;
    }

    CANFrameBuilder::XLFrameBuilder &CANFrameBuilder::XLFrameBuilder::setAF(std::uint32_t af) noexcept {
        rFrame.af = af;
        return *this;
    }

    CANFrameBuilder::XLFrameBuilder &CANFrameBuilder::XLFrameBuilder::setData(std::string_view data) noexcept {
        return setData(data.data(), data.size());
    }

    CANFrameBuilder::XLFrameBuilder &CANFrameBuilder::XLFrameBuilder::setData(const void *data, std::size_t size) noexcept {
        const auto len = ((size > CANXL_MAX_DLEN) ? CANXL_MAX_DLEN : size);

        rFrame.len = len;
        std::memcpy(rFrame.data, data, len);

        return *this;
    }

    CANFrame CANFrameBuilder::XLFrameBuilder::get() noexcept {
        return rFrame;
    }
#endif

    CANFrameBuilder::FrameBuilder CANFrameBuilder::createFrame() noexcept {
        frame = can_frame{};
        return {std::get<can_frame>(frame)};
    }

    CANFrameBuilder::FDFrameBuilder CANFrameBuilder::createFdFrame() noexcept {
        frame = canfd_frame{};
        return {std::get<canfd_frame>(frame)};
    }

#ifdef CANXL_MTU
    CANFrameBuilder::XLFrameBuilder CANFrameBuilder::createXLFrame() noexcept {
        frame = canxl_frame{};
        return {std::get<canxl_frame>(frame)};
    }
#endif
}
