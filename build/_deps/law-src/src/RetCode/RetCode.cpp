#include <system_error>

#include "law/RetCode/RetCode.h"

namespace law {
    void throwErrno() {
        throw std::system_error(errno, std::system_category());
    }

    void throwLawError(RetCode rc) {
        throw std::system_error(int(rc), std::generic_category());
    }
}
