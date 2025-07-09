#include <arpa/inet.h>

#include "law/RetCode/RetCode.h"
#include "law/Socket/Net/InternetAddress/InternetAddress.h"

namespace law::socket::net {
    InternetAddress::InternetAddress(std::uint16_t p) noexcept {
        nativeInternetAddress = makeInternetAddress({.s_addr = INADDR_ANY}, p);
    }

    InternetAddress::InternetAddress(std::string_view address, std::uint16_t port) {
        if (!applyAddress(address, port)) {
            throwErrno();
        }
    }

    InternetAddress::InternetAddress(const sockaddr_in &nativeAddress) noexcept:
            nativeInternetAddress(nativeAddress) {}

    InternetAddress::InternetAddress(InternetAddress &&other) noexcept:
            nativeInternetAddress(other.nativeInternetAddress) {}

    InternetAddress &InternetAddress::operator=(const InternetAddress &rhs) {
        if (this != &rhs) {
            nativeInternetAddress = rhs.nativeInternetAddress;
        }
        return *this;
    }

    InternetAddress &InternetAddress::operator=(InternetAddress &&rhs) noexcept {
        if (this != &rhs) {
            nativeInternetAddress = rhs.nativeInternetAddress;
        }
        return *this;
    }

    InternetAddress &InternetAddress::operator=(const sockaddr_in &rhs) noexcept {
        nativeInternetAddress = rhs;
        return *this;
    }

    InternetAddress &InternetAddress::operator=(sockaddr_in &&rhs) noexcept {
        nativeInternetAddress = rhs;
        return *this;
    }

    void InternetAddress::set(std::uint16_t p) noexcept {
        nativeInternetAddress.sin_port = htons(p);
    }

    Status InternetAddress::set(std::string_view address, std::uint16_t p) noexcept {
        return applyAddress(address, p);
    }

    [[nodiscard]] std::string_view InternetAddress::getInternetAddress() const noexcept {
        return inet_ntoa(nativeInternetAddress.sin_addr);
    }

    [[nodiscard]] std::uint16_t InternetAddress::getPort() const noexcept {
        return nativeInternetAddress.sin_port;
    }

    [[nodiscard]] sockaddr_in InternetAddress::getNativeAddress() const noexcept {
        return nativeInternetAddress;
    }

    Status InternetAddress::applyAddress(std::string_view hostname, std::uint16_t port) noexcept {
        return detail::IpResolver::resolve(hostname)
                .transform(
                        [this, port](const detail::ResolvedIP &ipAsInAddr) noexcept {
                            nativeInternetAddress = makeInternetAddress(ipAsInAddr, port);
                        }
                );
    }

    [[nodiscard]] sockaddr_in InternetAddress::makeInternetAddress(in_addr addr, std::uint16_t port) const noexcept {
        return {
                .sin_family = AF_INET,
                .sin_port = htons(port),
                .sin_addr = addr,
                .sin_zero = {0}
        };
    }
}
