#pragma once

#include "J1939Types.h"

#include "law/Socket/Base/SocketBase.h"

namespace law::socket::can::detail {
    class J1939Base : public socket::detail::SocketBase {
    private:
        int ifIndex = -1;

    public:
        J1939Base() noexcept = default;

        explicit J1939Base(
            std::string_view canIfName,
            std::uint8_t address = J1939_NO_ADDR,
            std::uint32_t pgn = J1939_NO_PGN,
            std::uint64_t name = J1939_NO_NAME
        );

        Status reattach() noexcept;

        Status reattach(
            std::string_view canIfName,
            std::uint8_t address = J1939_NO_ADDR,
            std::uint32_t pgn = J1939_NO_PGN,
            std::uint64_t name = J1939_NO_NAME
        ) noexcept;

        Status attach(
            std::string_view canIfName,
            std::uint8_t address = J1939_NO_ADDR,
            std::uint32_t pgn = J1939_NO_PGN,
            std::uint64_t name = J1939_NO_NAME
        ) noexcept;

        Status detach() noexcept;

        [[nodiscard]] Status connect(std::uint8_t addr, pgn_t pgn) const noexcept;

        Status applyFilter(const J1939Filter &filter) const noexcept;

        Status applyFilter(J1939FilterSequence filterSequence) const noexcept;

        Status setSendPriority(J1939SendPriority priority) const noexcept;

        J1939SendPriorityRet getSendPriority() const noexcept;

        Status broadcast(bool status) const noexcept;

        BoolRet broadcast() const noexcept;

        Status promiscuous(bool status) const noexcept;

        BoolRet promiscuous() const noexcept;

        WriteCountRet write(const void *extBuffer, std::size_t size) const noexcept;

        WriteCountRet write(const void *extBuffer, std::size_t size, pgn_t dest) const noexcept;

        WriteCountRet write(std::string_view data) const noexcept;

        WriteCountRet write(std::string_view data, pgn_t dest) const noexcept;

        WriteCountRet writev(IoVectorSet set) noexcept;

        WriteCountRet writev(IoVectorSet set, pgn_t dest) noexcept;

        ReadCountRet read(void *extBuffer, std::size_t readCount) const noexcept;

        ReadResultRet readFrom(void *extBuffer, std::size_t readCount) const noexcept;

    private:
        Status bind(const sockaddr_can &addr) const noexcept;
    };
}
