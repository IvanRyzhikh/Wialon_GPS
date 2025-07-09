#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "law/RetCode/RetCode.h"
#include "law/File/Base/FileBase.h"

namespace law::file::detail {
    FileBase::FileBase(std::string_view filename, OpenOption options, OpenMode modes):
        Base(::open(filename.data(), options.getAsIntegral(), modes.getAsIntegral())) {
        if (!operator bool()) {
            throwErrno();
        }
    }

    [[nodiscard]] FileModesRet FileBase::getPermissions() const noexcept {
        struct stat st { };
        if (fstat(this->getNativeHandler(), &st)) {
            return fromErrno<OpenMode>();
        }
        return OpenMode((st.st_mode & 0777));
    }

    Status FileBase::open(std::string_view filename, const OpenOption &options, const OpenMode &modes) noexcept {
        int fd = ::open(filename.data(), options.getAsIntegral(), modes.getAsIntegral());
        if (fd == -1) {
            return fromErrno();
        }

        setNativeHandler(&fd);
        return {};
    }

    WriteCountRet FileBase::write(std::string_view buffer) noexcept {
        return write(buffer.data(), buffer.size());
    }

    WriteCountRet FileBase::write(const void *buffer, std::size_t bufferSize) noexcept {
        auto bytesWrittenCount = ::write(getNativeHandler(), buffer, bufferSize);
        if (bytesWrittenCount == -1) {
            return fromErrno<WriteCount>();
        }

        return bytesWrittenCount;
    }

    [[nodiscard]] Status FileBase::flush() const noexcept {
        return ((fsync(this->getNativeHandler()) == -1) ? fromErrno() : Status());
    }

    [[nodiscard]] FileSizeRet FileBase::getFileSize() const noexcept {
        struct stat st{};
        if (fstat(this->getNativeHandler(), &st) == -1) {
            return fromErrno<FileSize>();
        }
        return st.st_size;
    }

    [[nodiscard]] Status FileBase::resize(std::size_t newSize) const noexcept {
        return ((ftruncate(this->getNativeHandler(), long(newSize)) == -1) ? fromErrno() : Status());
    }

    Status FileBase::setPosition(std::size_t pos) noexcept {
        return lseek(this->getNativeHandler(), int(pos), SEEK_SET) == -1 ? fromErrno() : Status();
    }

    ReadCountRet FileBase::read(void *extBuffer, std::size_t size) const noexcept {
        const auto recBytes = ::read(getNativeHandler(), extBuffer, size);
        if (recBytes == -1) {
            return fromErrno<ReadCount>();
        }

        return recBytes;
    }
}