#pragma once

#include <array>

#include "TcpClientBase.h"
#include "law/RetCode/RetCode.h"
#include "law/Socket/ReadOption.h"

namespace law::socket::net::tcp {
    template <std::size_t bufSize>
    class StaticTcpClient final : public detail::TcpClientBase {
    private:
        using Base = detail::TcpClientBase;
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
                size = bufSize;
            }

            return Base::read(readBuffer.data(), size, ro)
                    .and_then(
                            [this](std::size_t recBytes) noexcept -> BlobViewRet {
                                readBuffer[recBytes] = '\0';
                                return std::string_view{readBuffer.data(), std::string_view::size_type(recBytes)};
                            }
                    );
        }
    };

    class TcpClient final : public detail::TcpClientBase {
    private:
        using Base = detail::TcpClientBase;

    public:
        using Base::Base;

        using Base::read;

        [[nodiscard]] BlobRet read(const ReadOption &ro = ReadOption(0)) const noexcept;

        [[nodiscard]] BlobRet read(std::size_t size, const ReadOption &ro = ReadOption(0)) const noexcept;
    };

    using ExtBufferTcpClient = detail::TcpClientBase;
}
