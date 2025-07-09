#include <sys/socket.h>

#include "law/RetCode/RetCode.h"
#include "law/Socket/Net/TCP/Server/TcpConnectionBase.h"

namespace law::socket::net::tcp::detail {
    TcpConnectionBase::TcpConnectionBase(int fd) noexcept: fd::FileDescriptor(fd) {}

    TcpConnectionBase::TcpConnectionBase(int fd, const sockaddr_in &addr) noexcept:
            fd::FileDescriptor(fd), selfAddress(addr) {}

    WriteCountRet TcpConnectionBase::write(std::string_view data, const SendOption &so) const noexcept {
        return write(data.data(), data.size(), so);
    }

    WriteCountRet TcpConnectionBase::write(const void *buf, std::size_t size, const SendOption &so) const noexcept {
        auto writtenBytesCount = ::send(getNativeHandler(), buf, size, so.getAsIntegral());
        if (writtenBytesCount == -1) {
            return fromErrno<WriteCount>();
        }

        return writtenBytesCount;
    }

    WriteCountRet TcpConnectionBase::writev(IoVectorSet set) noexcept {
        auto bytesWrittenCount = ::writev(getNativeHandler(), set.data(), int(set.size()));
        if (bytesWrittenCount == -1) {
            return fromErrno<WriteCount>();
        }

        return bytesWrittenCount;
    }

    [[nodiscard]] ReadCountRet
    TcpConnectionBase::read(void *extBuffer, std::size_t readCount, const ReadOption &ro) const noexcept {
        const auto recBytes = recv(getNativeHandler(), extBuffer, readCount, ro.getAsIntegral());
        if (recBytes == -1) {
            return fromErrno<ReadCount>();
        }

        return recBytes;
    }

    Status TcpConnectionBase::destroy() noexcept {
        return close();
    }

    const sockaddr_in &TcpConnectionBase::getAddress() const noexcept {
        return selfAddress;
    }
}