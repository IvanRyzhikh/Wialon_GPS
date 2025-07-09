#pragma once

#include <array>

#include "law/Utils.h"
#include "UdsUdpServerBase.h"
#include "law/Socket/Unix/UDP/UdsUdpTypes.h"

namespace law::socket::uds::udp {
    template <std::size_t bufSize>
    class StaticUdsUdpServer final : public detail::UdsUdpServerBase {
    private:
        using Base = detail::UdsUdpServerBase;
        using ReadBuffer = std::array<char, bufSize + 1>;

    private:
        ReadBuffer readBuffer;

    public:
        using Base::Base;

        using Base::read;

        [[nodiscard]] ClientPayloadViewRet read(const ReadOption &ro = ReadOption(0)) noexcept {
            return utils::getAvailableBytesForRead(getNativeHandler())
                    .and_then(
                            [this, &ro](std::size_t availableBytes) noexcept {
                                return (
                                        (availableBytes == 0) ?
                                        read(bufSize, ro) :
                                        read((availableBytes > bufSize ? bufSize : availableBytes), ro)
                                );
                            }
                    );
        }

        [[nodiscard]] ClientPayloadViewRet
        read(std::size_t size, const ReadOption &ro = ReadOption(0)) noexcept {
            if (size > bufSize) {
                return fromLawError<ClientPayloadView>(RetCode::NO_AVAILABLE_SPACE);
            }

            return Base::read(readBuffer.data(), size, ro)
                    .transform(
                            [this](const ReadResult &readResult) noexcept {
                                auto &[clientAddress, recBytes] = readResult;
                                readBuffer[recBytes] = '\0';
                                return makeClientPayload(clientAddress, recBytes);
                            }
                    );
        }

    private:
        [[nodiscard]] ClientPayloadView
        makeClientPayload(const sockaddr_un &nativeAddress, std::size_t recBytes) const noexcept {
            ClientPayloadView ret;
            ret.address.setNativeAddress(nativeAddress);
            ret.blob = {readBuffer.data(), recBytes};
            return ret;
        }
    };

    class UdsUdpServer final : public detail::UdsUdpServerBase {
    private:
        using Base = detail::UdsUdpServerBase;

    public:
        using Base::Base;

        using Base::read;

        [[nodiscard]] ClientPayloadRet read(const ReadOption &ro = ReadOption(0)) noexcept;

        [[nodiscard]] ClientPayloadRet read(std::size_t size, const ReadOption &ro = ReadOption(0)) const noexcept;

    private:
        [[nodiscard]] static ClientPayload
        makeClientPayload(const sockaddr_un &nativeAddress, std::string &&payload) noexcept;
    };

    using ExtBufferUdsUdpServer = detail::UdsUdpServerBase;
}
