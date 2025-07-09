#pragma once

#include <unistd.h>

#include "law/Types.h"
#include "law/Pipe/PipeFlags.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::pipe::detail {
    class PipeBase : public fd::FileDescriptor {
    private:
        pid_t childPid = -1;
        int fd[2] = {-1, -1};

    public:
        PipeBase() noexcept = default;

        explicit PipeBase(std::string_view cmd, const PipeFlags &openFlags = PipeFlags(0));

        PipeBase(PipeBase &&other) noexcept;

        ~PipeBase() noexcept override;

        Status init(std::string_view cmd, const PipeFlags &openFlags = PipeFlags(0)) noexcept;

        WriteCountRet write(const void *buf, std::size_t size) noexcept;

        WriteCountRet write(std::string_view buffer) noexcept;

        [[nodiscard]] PipeSizeRet getPipeSize() const noexcept;

        ReadCountRet read(void *extBuffer, std::size_t size) const noexcept;

        [[nodiscard]] int getInputFileDescriptor() const noexcept;

        [[nodiscard]] int getOutputFileDescriptor() const noexcept;

        [[nodiscard]] pid_t getChildPid() const noexcept;

        void wait() noexcept;

    private:
        void closeDescriptors() noexcept;
    };
}
