#include "law/Socket/Net/UDP/Client/UdpClient.h"

namespace law::socket::net::udp {
    BlobRet UdpClient::read(const ReadOption &ro) const noexcept {
        return utils::getAvailableBytesForRead(getNativeHandler())
                .and_then(
                        [this, &ro](std::size_t availableBytes) noexcept {
                            return (availableBytes == 0) ? read(kDefaultReadCount, ro) : read(availableBytes, ro);
                        }
                );
    }

    BlobRet UdpClient::read(std::size_t size, const ReadOption &ro) const noexcept {
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