#pragma once

#include <sys/eventfd.h>

#include "law/RetCode/RetCode.h"

namespace law::efd {
    /** Just alias for glibc eventfd_t type */
    using Value = eventfd_t;

    using ValueRet = CustomError<eventfd_t>;
}