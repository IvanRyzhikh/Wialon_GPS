#include "law/Socket/Unix/UDP/Server/UdsUdpServer.h"

namespace law::socket::uds::udp {
    ClientPayloadRet UdsUdpServer::read(const ReadOption &ro) noexcept {
        return utils::getAvailableBytesForRead(getNativeHandler())
                .and_then(
                        [this, &ro](std::size_t availableBytes) noexcept {
                            return (availableBytes == 0) ? read(kDefaultReadCount, ro) : read(availableBytes, ro);
                        }
                );
    }

    ClientPayloadRet UdsUdpServer::read(std::size_t size, const ReadOption &ro) const noexcept {
        std::string payload(size, '\0');
        return Base::read(payload.data(), size, ro)
                .and_then(
                        [&payload](const ReadResult &readResult) noexcept -> ClientPayloadRet {
                            auto &[clientAddress, recBytes] = readResult;
                            payload.resize(recBytes);
                            return makeClientPayload(clientAddress, std::move(payload));
                        }
                );
    }

    ClientPayload UdsUdpServer::makeClientPayload(const sockaddr_un &nativeAddress, std::string &&payload) noexcept {
        ClientPayload ret;
        ret.address.setNativeAddress(nativeAddress);
        ret.blob = std::move(payload);
        return ret;
    }
}
