#include "law/Socket/CAN/J1939/J1939.h"

namespace law::socket::can {
    [[nodiscard]] BlobRet J1939::read(std::size_t size) const noexcept {
        std::string ret(size, '\0');

        EXPECTED_AUTO(recBytes, Base::read(ret.data(), size));

        ret.resize(recBytes);
        return ret;
    }

    [[nodiscard]] ReadResultBlobRet J1939::readFrom(std::size_t size) const noexcept {
        std::string ret(size, '\0');

        EXPECTED_AUTO(readResult, Base::readFrom(ret.data(), size));

        ret.resize(readResult.recBytes);
        return ReadResultBlob{readResult.sourceAddress, std::move(ret)};
    }
}