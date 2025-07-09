#pragma once

#include <netinet/in.h>

#include "law/Socket/SendOption.h"
#include "law/Socket/ReadOption.h"
#include "law/Socket/Net/TCP/TcpTypes.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::socket::net::tcp::detail {
    class TcpConnectionBase : public fd::FileDescriptor {
    private:
        sockaddr_in selfAddress;

    public:
        explicit TcpConnectionBase(int fd) noexcept;

        explicit TcpConnectionBase(int fd, const sockaddr_in &addr) noexcept;

        WriteCountRet write(std::string_view data, const SendOption &so = NoSignal()) const noexcept;

        WriteCountRet write(const void *buf, std::size_t size, const SendOption &so = NoSignal()) const noexcept;

        WriteCountRet writev(IoVectorSet set) noexcept;

        [[nodiscard]] ReadCountRet read(void *extBuffer, std::size_t readCount, const ReadOption &ro = ReadOption(0)) const noexcept;

        Status destroy() noexcept;

        const sockaddr_in &getAddress() const noexcept;
    };
}
