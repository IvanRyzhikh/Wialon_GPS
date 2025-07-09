#include <boost/assert.hpp>

#include "law/Socket/Net/TCP/Client/TcpClientBase.h"
#include "law/Socket/Net/TCP/Server/TcpServerBase.h"

namespace law::socket::net::tcp::detail {
    TcpServerBase::TcpServerBase(InternetAddress &&addr) : address(std::move(addr)) {
        openSocket();
    }

    TcpServerBase::TcpServerBase(const InternetAddress &addr) : address(addr) {
        openSocket();
    }

    Status TcpServerBase::start(std::uint16_t maxConnectionInQueue) const noexcept {
        return reuseAddress()
                .and_then(
                        [this] noexcept {
                            return reusePort();
                        }
                )
                .and_then(
                        [this, maxConnectionInQueue] noexcept {
                            const auto addr = address.getNativeAddress();
                            if (bind(getNativeHandler(), reinterpret_cast<const sockaddr *>(&addr), sizeof(addr)) == -1) {
                                return fromErrno();
                            }

                            if (listen(getNativeHandler(), maxConnectionInQueue) == -1) {
                                return fromErrno();
                            }

                            return Status();
                        }
                );
    }

    [[nodiscard]] TcpServerBase::ConnDescriptorRet
    TcpServerBase::acceptConnection(const AcceptOption &ao) const noexcept {
        ConnDescriptor ret{};
        socklen_t connAddressLength = sizeof(sockaddr_in);

        ret.fd =
                ::accept4(getNativeHandler(),
                          reinterpret_cast<sockaddr *>(&ret.address), &connAddressLength,
                          ao.getAsIntegral()
                );
        if (ret.fd == -1) {
            return fromErrno<ConnDescriptor>();
        }

        BOOST_ASSERT(connAddressLength == sizeof(sockaddr_in));
        return ret;
    }
}