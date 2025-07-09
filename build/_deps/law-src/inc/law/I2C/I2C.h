#pragma once

#include "law/Utils.h"
#include "Base/I2CBase.h"

namespace law::i2c {
    template <std::size_t bufSize>
    class StaticI2C final : public detail::I2CBase {
    private:
        using Base = I2CBase;
        using ReadBuffer = std::array<char, bufSize + 1>;

    private:
        ReadBuffer readBuffer;

    public:
        using Base::Base;

        using Base::read;

        [[nodiscard]] BlobViewRet read() noexcept {
            return read(bufSize);
        }

        [[nodiscard]] BlobViewRet read(std::size_t size) noexcept {
            if (size > bufSize) {
                size = bufSize;
            }

            return Base::read(readBuffer.data(), size)
            .and_then(
            [this](std::size_t recBytes) noexcept -> BlobViewRet {
            readBuffer[recBytes] = '\0';
            return std::string_view{readBuffer.data(), std::string_view::size_type(recBytes)};
            }
            );
        }
    };

    class I2C final : public detail::I2CBase {
    private:
        using Base = I2CBase;

    public:
        using Base::Base;

        using Base::read;

        [[nodiscard]] BlobRet read() noexcept;

        [[nodiscard]] BlobRet read(std::size_t size) noexcept;
    };

    using ExtBufferI2C = detail::I2CBase;
}
