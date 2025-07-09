#pragma once

#include <chrono>
#include <concepts>
#include <string_view>

#include "Types.h"
#include "FileDescriptor/FileDescriptorFwd.h"

namespace law::utils {
    namespace detail {
        static constexpr int kOptionEnableVal = 1;
        static constexpr int kOptionDisableVal = 0;
    }

#define LAW_INTERNAL_ERROR_CHECK(expr) ((expr) == -1) ? fromErrno() : Status()

    BlobViewRet getHostname() noexcept;

    bool validateDNS(std::string_view str) noexcept;

    AvailableDataSizeRet getAvailableBytesForRead(const fd::FileDescriptor &fd) noexcept;

    AvailableDataSizeRet getAvailableBytesForRead(int fd) noexcept;
}
