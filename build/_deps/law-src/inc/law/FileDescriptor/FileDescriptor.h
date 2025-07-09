#pragma once

#include <sys/ioctl.h>

#include "law/Types.h"
#include "law/Utils.h"
#include "FileDescriptorFwd.h"
#include "law/RetCode/RetCode.h"

namespace law::fd {
    class FileDescriptor {
    private:
        int fd = -1;

    public:
        FileDescriptor() noexcept = default;

        explicit FileDescriptor(int fd);

        FileDescriptor(const FileDescriptor &) = delete;

        FileDescriptor &operator=(const FileDescriptor &) = delete;

        FileDescriptor(FileDescriptor &&other) noexcept;

        FileDescriptor &operator=(FileDescriptor &&rhs) noexcept;

        virtual ~FileDescriptor() noexcept;

        /**
         * @return file descriptor as int
         */
        [[nodiscard]] int getNativeHandler() const noexcept;

        /**
         * @return true, if @var fd != -1
         */
        explicit operator bool() const noexcept;

        Status applyOption(int request, const auto ...params) const noexcept {
            return LAW_INTERNAL_ERROR_CHECK(ioctl(fd, request, params...));
        }

        Status readOption(int request, auto ...params) const noexcept {
            return LAW_INTERNAL_ERROR_CHECK(ioctl(fd, request, params...));
        }

        Status nonblock(bool state) const noexcept;

        [[nodiscard]] BoolRet nonblock() const noexcept;

    protected:
        void setNativeHandler(int *) noexcept;

        Status close() noexcept;
    };
}
