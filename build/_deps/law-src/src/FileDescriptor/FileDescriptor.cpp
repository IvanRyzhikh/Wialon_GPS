#include <fcntl.h>
#include <unistd.h>

#include "law/Utils.h"
#include "law/RetCode/RetCode.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::fd {
    FileDescriptor::FileDescriptor(int fd) : fd(fd) {}

    FileDescriptor::FileDescriptor(FileDescriptor &&fileDescriptor) noexcept: fd(fileDescriptor.fd) {
        fileDescriptor.fd = -1;
    }

    FileDescriptor &FileDescriptor::operator=(FileDescriptor &&rhs) noexcept {
        if (this != &rhs) {
            fd = rhs.fd;
            rhs.fd = -1;
        }
        return *this;
    }

    FileDescriptor::~FileDescriptor() noexcept {
        close();
    }

    [[nodiscard]] int FileDescriptor::getNativeHandler() const noexcept {
        return fd;
    }

    FileDescriptor::operator bool() const noexcept {
        return fd != -1;
    }

    Status FileDescriptor::nonblock(bool state) const noexcept {
        auto flags = fcntl(fd, F_GETFL, 0);
        if (flags == -1) {
            return fromErrno();
        }

        if (state & (flags & O_NONBLOCK)) {
            return {};
        }

        if (!state & !(flags & O_NONBLOCK)) {
            return {};
        }

        flags = state ? (flags | O_NONBLOCK) : (flags & (~O_NONBLOCK));
        return LAW_INTERNAL_ERROR_CHECK(fcntl(fd, F_SETFL, flags));
    }

    [[nodiscard]] BoolRet FileDescriptor::nonblock() const noexcept {
        const auto flags = fcntl(fd, F_GETFL, 0);
        if (flags == -1) {
            return fromErrno<Bool>();
        }
        return (flags & O_NONBLOCK);
    }

    void FileDescriptor::setNativeHandler(int *handler) noexcept {
        fd = *handler;
        *handler = -1;
    }

    Status FileDescriptor::close() noexcept {
        if (fd == -1) {
            return {};
        }

        if (::close(fd) == -1) {
            return fromErrno();
        }

        fd = -1;
        return {};
    }
}