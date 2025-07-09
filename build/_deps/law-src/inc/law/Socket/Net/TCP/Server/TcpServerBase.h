#pragma once

#include "AcceptOption.h"
#include "law/Socket/SendOption.h"
#include "law/Socket/Net/TCP/Base/TcpSocketBase.h"
#include "law/Socket/Net/InternetAddress/InternetAddress.h"

namespace law::socket::net::tcp::detail {
    class TcpServerBase : public TcpSocketBase {
    private:
        using Base = TcpSocketBase;

    protected:
        struct ConnDescriptor {
            int fd;
            sockaddr_in address;
        };
        using ConnDescriptorRet = CustomError<ConnDescriptor>;

    private:
        InternetAddress address{};

    public:
        TcpServerBase() noexcept = default;

        TcpServerBase(InternetAddress &&addr);

        TcpServerBase(const InternetAddress &addr);

        [[nodiscard]] Status start(std::uint16_t maxConnectionInQueue = SOMAXCONN) const noexcept;

    protected:
        [[nodiscard]] ConnDescriptorRet acceptConnection(const AcceptOption &ao = AcceptOption(0)) const noexcept;
    };
}
