#include <string>

#include "law/Pipe/Pipe.h"

namespace law::pipe {
    [[nodiscard]] BlobRet Pipe::read() const noexcept {
        return getPipeSize()
                .and_then(
                        [this](PipeSize pipeSize) noexcept {
                            return read(pipeSize);
                        }
                );
    }

    [[nodiscard]] BlobRet Pipe::read(std::size_t size) const noexcept {
        std::string ret(size, '\0');
        return Base::read(ret.data(), size)
                .and_then(
                        [&ret](std::size_t recBytes) noexcept -> BlobRet {
                            ret.resize(recBytes);
                            return ret;
                        }
                );
    }
}