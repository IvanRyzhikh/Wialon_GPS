#pragma once

#include <array>

#include "law/Types.h"
#include "Base/PipeBase.h"
#include "law/RetCode/RetCode.h"

namespace law::pipe {
    template <std::size_t bufSize>
    class StaticPipe final : public detail::PipeBase {
    private:
        using Base = detail::PipeBase;

        using ReadBuffer = std::array<char, bufSize + 1>;

    private:
        ReadBuffer readBuffer;

    public:
        using Base::read;

        [[nodiscard]] BlobViewRet read() noexcept {
            return this->getPipeSize()
                    .and_then(
                            [this](std::size_t pipeSize) noexcept {
                                return read((pipeSize > bufSize ? bufSize : pipeSize));
                            }
                    );
        }

        [[nodiscard]] BlobViewRet read(std::size_t size) noexcept {
            if (size > bufSize) {
                return std::unexpected(RetCode::NO_AVAILABLE_SPACE);
            }
            return Base::read(readBuffer.data(), size)
                    .and_then(
                            [this](std::size_t recBytes) noexcept ->  BlobViewRet {
                                readBuffer[recBytes] = '\0';
                                return std::string_view{readBuffer.data(), std::string_view::size_type(recBytes)};
                            }
                    );
        }
    };

    class Pipe final : public detail::PipeBase {
    private:
        using Base = detail::PipeBase;

    public:
        using Base::read;

        [[nodiscard]] BlobRet read() const noexcept;

        [[nodiscard]] BlobRet read(std::size_t size) const noexcept;
    };

    using ExtBufferPipe = detail::PipeBase;
}
