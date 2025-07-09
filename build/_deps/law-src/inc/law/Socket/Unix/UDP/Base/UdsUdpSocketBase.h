#pragma once

#include <etl/string.h>

#include "law/Socket/SendOption.h"
#include "law/Socket/Unix/UDP/UdsUdpTypes.h"
#include "law/Socket/Unix/Base/UnixSocketBase.h"
#include "law/Socket/Unix/UnixAddress/UnixAddress.h"

namespace law::socket::uds::udp::detail {
    class UdsUdpSocketBase : public uds::detail::UnixSocketBase {
    private:
        using UnixPath = etl::string<sizeof(sockaddr_un)>;

    public:
        [[nodiscard]] WriteCountRet
        write(std::string_view data, const UnixAddress &addr, const SendOption &so = Confirm()) const noexcept;

        [[nodiscard]] WriteCountRet write(const msghdr &message, const UnixAddress &dest) const noexcept;

        WriteCountRet
        write(const void *buf, std::size_t size, const UnixAddress &addr, const SendOption &so = Confirm()) const noexcept;

    protected:
        Status makeSocket() noexcept;

        static UnixPath makeRandomUnixPath() noexcept;
    };
}
