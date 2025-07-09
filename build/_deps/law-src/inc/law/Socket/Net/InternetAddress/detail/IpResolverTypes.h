#pragma once

#include <netinet/in.h>

namespace law::socket::net::detail {
    using ResolvedIP = in_addr;
    using ResolvedIPRet = CustomError<in_addr>;
}
