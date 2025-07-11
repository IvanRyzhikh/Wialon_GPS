#pragma once

#include "IpResolverTypes.h"

namespace law::socket::net::detail {
    class IpResolver {
    public:
        static ResolvedIPRet resolve(std::string_view address) noexcept;

    private:
        static bool isHostname(std::string_view address) noexcept;

        static bool isLocalhost(std::string_view hostname) noexcept;

        static std::string_view extractDomain(std::string_view url);

        static ResolvedIPRet hostnameToIp(std::string_view hostname) noexcept;
    };
}
