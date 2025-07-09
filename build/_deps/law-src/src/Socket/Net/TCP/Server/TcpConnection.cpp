#include "law/Socket/Net/TCP/Server/TcpConnection.h"

namespace law::socket::net::tcp {
    [[nodiscard]] BlobRet TcpConnection::read(const ReadOption &ro) noexcept {
        return utils::getAvailableBytesForRead(getNativeHandler())
                .and_then(
                        [this, &ro](std::size_t availableBytes) noexcept {
                            static constexpr std::size_t kDefaultReadCount = 8192;
                            return (availableBytes == 0) ? read(kDefaultReadCount, ro) : read(availableBytes, ro);
                        }
                );
    }

    [[nodiscard]] BlobRet TcpConnection::read(std::size_t size, const ReadOption &ro) noexcept {
        std::string ret(size, '\0');
        return Base::read(ret.data(), size, ro)
                .and_then(
                        [&ret](std::size_t recBytes) noexcept -> BlobRet {
                            ret.resize(recBytes);
                            return ret;
                        }
                );
    }
}