#pragma once

#include <array>

#include "law/Utils.h"
#include "law/UART/Base/UartBase.h"
#include "law/RetCode/RetCode.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::uart::rs232 {
    template <std::size_t bufSize>
    class StaticRS232 final : public law::uart::detail::UartBase {
    private:
        using Base = law::uart::detail::UartBase;
        using ReadBuffer = std::array<char, bufSize + 1>;

    private:
        ReadBuffer readBuffer;

    public:
        using Base::Base;

        using Base::read;

        [[nodiscard]] BlobViewRet read() noexcept {
            return utils::getAvailableBytesForRead(getNativeHandler())
                    .and_then(
                            [this](std::size_t availableBytes) noexcept {
                                return (availableBytes == 0) ? read(getDefaultReadCount()) : read(
                                        (availableBytes > bufSize ? bufSize : availableBytes));
                            }
                    );
        }

        [[nodiscard]] BlobViewRet read(std::size_t size) noexcept {
            if (size > bufSize) {
                return fromLawError<BlobView>(RetCode::NO_AVAILABLE_SPACE);
            }

            return Base::read(readBuffer.data(), size)
                    .and_then(
                            [this](std::size_t recBytes) noexcept -> BlobViewRet {
                                readBuffer[recBytes] = '\0';
                                return std::string_view{readBuffer.data(), recBytes};
                            }
                    );
        }
    };

    class RS232 final : public law::uart::detail::UartBase {
    private:
        using Base = law::uart::detail::UartBase;

    public:
        using Base::Base;

        using Base::read;

        [[nodiscard]] BlobRet read() const noexcept;

        [[nodiscard]] BlobRet read(std::size_t size) const noexcept;
    };

    using ExtBufferRS232 = law::uart::detail::UartBase;
}