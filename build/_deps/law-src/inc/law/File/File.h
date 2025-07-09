#pragma once

#include <string>
#include <etl/string.h>

#include "law/Types.h"
#include "law/Utils.h"
#include "Base/FileBase.h"
#include "law/RetCode/RetCode.h"

namespace law::file {
    template <std::size_t bufSize>
    class StaticFile final : public detail::FileBase {
    private:
        using Base = detail::FileBase;

        using ReadBuffer = std::array<char, bufSize + 1>;

    private:
        ReadBuffer readBuffer;

    public:
        StaticFile() noexcept = default;

        explicit
        StaticFile(std::string_view fn, OpenOption openFlag = ReadWrite(), OpenMode modes = OwnerReadWriteExec()) :
                Base(fn, openFlag, modes) {}

        using Base::read;

        /**
         * @return all file content as string - On success
         * @return EC::X_EAGAIN
         * @return EC::X_EWOULDBLOCK
         * @return EC::X_EBADF
         * @return EC::X_EFAULT
         * @return EC::X_EINTR
         * @return EC::X_EINVAL
         * @return EC::X_EIO
         * @return EC::X_EISDIR
         * @return EC::X_EOVERFLOW
         * */
        [[nodiscard]] BlobViewRet read() noexcept {
            return this->getFileSize()
                    .and_then(
                            [this](std::size_t fileSize) noexcept {
                                return read(fileSize);
                            }
                    );
        }

        /**
         * @return all file content as string - On success
         * @return EC::X_EAGAIN
         * @return EC::X_EWOULDBLOCK
         * @return EC::X_EBADF
         * @return EC::X_EFAULT
         * @return EC::X_EINTR
         * @return EC::X_EINVAL
         * @return EC::X_EIO
         * @return EC::X_EISDIR
         * @return EC::X_EOVERFLOW
         * */
        [[nodiscard]] BlobViewRet read(std::size_t size) noexcept {
            if (size > bufSize) {
                return fromLawError<BlobView>(RetCode::NO_AVAILABLE_SPACE);
            }
            return Base::read(readBuffer.data(), size)
                    .transform(
                            [this](std::size_t recBytes) noexcept {
                                readBuffer[recBytes] = '\0';
                                return std::string_view{readBuffer.data(), std::string_view::size_type(recBytes)};
                            }
                    );
        }
    };

    class File final : public detail::FileBase {
    private:
        using Base = detail::FileBase;

    public:
        File() noexcept = default;

        template <StringLike T>
        explicit
        File(T &&filename, OpenOption openFlag = ReadWrite(), OpenMode modes = OwnerReadWriteExec()) :
                Base(filename, openFlag, modes) {}

        using Base::read;

        /**
         * @return all file content as string - On success
         * @return EC::X_EAGAIN
         * @return EC::X_EWOULDBLOCK
         * @return EC::X_EBADF
         * @return EC::X_EFAULT
         * @return EC::X_EINTR
         * @return EC::X_EINVAL
         * @return EC::X_EIO
         * @return EC::X_EISDIR
         * @return EC::X_EOVERFLOW
         * */
        [[nodiscard]] BlobRet read() const noexcept;

        /**
         * @return all file content as string - On success
         * @return EC::X_EAGAIN
         * @return EC::X_EWOULDBLOCK
         * @return EC::X_EBADF
         * @return EC::X_EFAULT
         * @return EC::X_EINTR
         * @return EC::X_EINVAL
         * @return EC::X_EIO
         * @return EC::X_EISDIR
         * @return EC::X_EOVERFLOW
         * */
        [[nodiscard]] BlobRet read(std::size_t size) const noexcept;
    };

    using ExtBufferFile = detail::FileBase;
}
