#include "law/I2C/I2C.h"

namespace law::i2c {
    [[nodiscard]] BlobRet I2C::read() noexcept {
        static constexpr std::size_t kDefaultReadCount = 1;
        return read(kDefaultReadCount);
    }

    [[nodiscard]] BlobRet I2C::read(std::size_t size) noexcept {
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
