#pragma once

#include <errcode/errcode.h>

#include "J1939Base.h"
#include "law/Utils.h"
#include "J1939Types.h"

namespace law::socket::can {
    template <std::size_t bufSize>
    class StaticJ1939 final : public detail::J1939Base {
    private:
        using Base = J1939Base;
        using ReadBuffer = std::array<char, bufSize + 1>;

    private:
        ReadBuffer readBuffer;

    public:
        using Base::Base;

        using Base::read;
        using Base::readFrom;

        [[nodiscard]] BlobViewRet read(std::size_t size = bufSize) noexcept {
            if (size > bufSize) {
                return fromLawError<BlobView>(RetCode::NO_AVAILABLE_SPACE);
            }

            EXPECTED_AUTO(recBytes, Base::read(readBuffer.data(), size));

            readBuffer[recBytes] = '\0';
            return std::string_view{readBuffer.data(), std::string_view::size_type(recBytes)};
        }

        [[nodiscard]] ReadResultBlobViewRet readFrom(std::size_t size = bufSize) noexcept {
            if (size > bufSize) {
                return fromLawError<ReadResultBlobView>(RetCode::NO_AVAILABLE_SPACE);
            }

            EXPECTED_AUTO(readResult, Base::readFrom(readBuffer.data(), size));

            readBuffer[readResult.recBytes] = '\0';
            return ReadResultBlobView{
                readResult.sourceAddress,
                std::string_view{readBuffer.data(),
                                 std::string_view::size_type(readResult.recBytes)}
            };
        }
    };

    class J1939 final : public detail::J1939Base {
    private:
        using Base = detail::J1939Base;

    private:
        static constexpr std::uint16_t kJ1939FrameMaxLength = 1785;

    public:
        using Base::Base;

        using Base::read;
        using Base::readFrom;

        [[nodiscard]] BlobRet read(std::size_t size = kJ1939FrameMaxLength) const noexcept;

        [[nodiscard]] ReadResultBlobRet readFrom(std::size_t size = kJ1939FrameMaxLength) const noexcept;
    };

    using ExtBufferJ1939 = detail::J1939Base;
}