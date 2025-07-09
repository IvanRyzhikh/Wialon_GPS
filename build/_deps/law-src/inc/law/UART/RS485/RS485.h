#pragma once

#include <array>

#include "Base/RS485Base.h"
#include "law/FileDescriptor/FileDescriptor.h"
#include "law/RetCode/RetCode.h"
#include "law/UART/RS485/RS485Configuration.h"
#include "law/UART/UartConfiguration.h"
#include "law/Utils.h"

namespace law::uart::rs485 {
    template <std::size_t bufSize>
    class StaticRS485 final : public detail::RS485Base {
    private:
        using Base = detail::RS485Base;
        using ReadBuffer = std::array<char, bufSize + 1>;

    private:
        ReadBuffer readBuffer;

    public:
        using Base::Base;

        using Base::read;

        [[nodiscard]] BlobViewRet read() noexcept {
            return utils::getAvailableBytesForRead(Base::getNativeHandler())
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

    class RS485 final : public detail::RS485Base {
    private:
        using Base = detail::RS485Base;

    public:
        using Base::Base;

        using Base::read;

        [[nodiscard]] BlobRet read() const noexcept;

        [[nodiscard]] BlobRet read(std::size_t size) const noexcept;
    };

    using ExtBufferRS485 = detail::RS485Base;
}