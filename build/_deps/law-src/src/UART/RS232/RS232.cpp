#include "law/UART/RS232/RS232.h"

namespace law::uart::rs232 {
    BlobRet RS232::read() const noexcept {
        return utils::getAvailableBytesForRead(getNativeHandler())
                .and_then(
                        [this](std::size_t availableBytes) noexcept {
                            return (availableBytes == 0) ? read(getDefaultReadCount()) : read(availableBytes);
                        }
                );
    }

    BlobRet RS232::read(std::size_t size) const noexcept {
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