#pragma once

#include <array>

#include "law/Utils.h"
#include "law/RetCode/RetCode.h"
#include "law/Socket/Unix/UDP/Client/UdsUdpClientBase.h"

namespace law::socket::uds::udp {
    template <std::size_t bufSize>
    class StaticUdsUdpClient final : public detail::UdsUdpClientBase {
    private:
        using Base = detail::UdsUdpClientBase;
        using ReadBuffer = std::array<char, bufSize + 1>;

    private:
        ReadBuffer readBuffer;

    public:
        using Base::Base;

        using Base::read;

        [[nodiscard]] BlobViewRet read(const ReadOption &ro = ReadOption(0)) noexcept {
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

        [[nodiscard]] BlobViewRet read(std::size_t size, const ReadOption &ro = ReadOption(0)) noexcept {
            if (size > bufSize) {
                return fromLawError<BlobView>(RetCode::NO_AVAILABLE_SPACE);
            }

            return Base::read(readBuffer.data(), size, ro)
                    .and_then(
                            [this](std::size_t recBytes) noexcept -> BlobViewRet {
                                readBuffer[recBytes] = '\0';
                                return std::string_view{readBuffer.data(), recBytes};
                            }
                    );
        }
    };

    class UdsUdpClient final : public detail::UdsUdpClientBase {
    private:
        using Base = detail::UdsUdpClientBase;

    public:
        using Base::Base;

        using Base::read;

        [[nodiscard]] BlobRet read(const ReadOption &ro = ReadOption(0)) const noexcept;

        [[nodiscard]] BlobRet read(std::size_t size, const ReadOption &ro = ReadOption(0)) const noexcept;
    };

    using ExtBufferUdsUdpClient = detail::UdsUdpClientBase;
}
