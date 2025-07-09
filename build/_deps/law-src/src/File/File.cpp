#include "law/File/File.h"

namespace law::file {
    [[nodiscard]] BlobRet File::read() const noexcept {
        return getFileSize()
                .and_then(
                        [this](std::size_t fileSize) noexcept {
                            return read(fileSize);
                        }
                );
    }

    [[nodiscard]] BlobRet File::read(std::size_t size) const noexcept {
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