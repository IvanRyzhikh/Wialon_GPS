#include <fcntl.h>
#include <paths.h>
#include <unistd.h>
#include <sys/wait.h>

#include "law/Utils.h"
#include "law/RetCode/RetCode.h"
#include "law/Pipe/Base/PipeBase.h"

namespace law::pipe::detail {
    PipeBase::PipeBase(std::string_view cmd, const PipeFlags &openFlags) {
        if (!init(cmd, openFlags)) {
            throwErrno();
        }
    }

    PipeBase::PipeBase(PipeBase &&other) noexcept {
        fd[0] = other.fd[0];
        fd[1] = other.fd[1];
        childPid = other.childPid;

        other.fd[0] = -1;
        other.fd[1] = -1;
        other.childPid = -1;
    }

    PipeBase::~PipeBase() noexcept {
        if (childPid != -1) {
            wait();
        }
    }

    Status PipeBase::init(std::string_view cmd, const PipeFlags &flags) noexcept {
        if (::pipe2(fd, flags.getAsIntegral()) != 0) {
            return fromErrno();
        }

        pid_t pid;
        switch (pid = vfork()) {
            case -1: {
                return fromErrno();
            }
            case 0: {
                if ((dup2(fd[0], STDIN_FILENO) == -1) || (::close(fd[0]) == -1)) {
                    return fromErrno();
                }

                if ((dup2(fd[1], STDOUT_FILENO) == -1) || (::close(fd[1]) == -1)) {
                    return fromErrno();
                }

                if (execl(_PATH_BSHELL, "sh", "-c", cmd.data(), nullptr) == -1) {
                    return fromErrno();
                }
            }
            default: {
                break;
            }
        }

        childPid = pid;

        int inFdCopy = fd[0]; // dirty hack prevent fd[0] = -1
        setNativeHandler(&inFdCopy);

        return {};
    }

    WriteCountRet PipeBase::write(const void *buf, std::size_t size) noexcept {
        return write({reinterpret_cast<const char *>(buf), size});
    }

    WriteCountRet PipeBase::write(std::string_view buffer) noexcept {
        auto writtenBytesCount = ::write(fd[1], buffer.data(), buffer.size());
        if (writtenBytesCount == -1) {
            return fromErrno<WriteCount>();
        }

        return writtenBytesCount;
    }

    [[nodiscard]] PipeSizeRet PipeBase::getPipeSize() const noexcept {
        const auto pipeSize = fcntl(fd[0], F_GETPIPE_SZ);
        if (pipeSize == -1) {
            return fromErrno<PipeSize>();
        }

        return pipeSize;
    }

    ReadCountRet PipeBase::read(void *extBuffer, std::size_t size) const noexcept {
        const auto recBytes = ::read(fd[0], extBuffer, size);
        if (recBytes == -1) {
            return fromErrno<ReadCount>();
        }

        return recBytes;
    }

    int PipeBase::getOutputFileDescriptor() const noexcept {
        return fd[1];
    }

    int PipeBase::getInputFileDescriptor() const noexcept {
        return getNativeHandler();
    }

    void PipeBase::closeDescriptors() noexcept {
        if (fd[1] != -1) {
            ::close(fd[1]);
        }
    }

    pid_t PipeBase::getChildPid() const noexcept {
        return childPid;
    }

    void PipeBase::wait() noexcept {
        while (waitpid(childPid, nullptr, 0) == -1) {
            if (errno != EINTR) {
                break;
            }
        }

        if (fd[1] != -1) {
            ::close(fd[1]);
            this->close();
        }

        childPid = -1;
    }
}
