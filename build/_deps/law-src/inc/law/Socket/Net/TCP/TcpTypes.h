#pragma once

#include <span>
#include <sys/uio.h>
#include <netinet/tcp.h>

#include "law/RetCode/RetCode.h"

namespace law::socket::net::tcp {
    using SocketError = std::uint32_t;
    using SocketErrorRet = CustomError<SocketError>;

    using TcpInfo = tcp_info;
    using TcpInfoRet = CustomError<tcp_info>;
}
