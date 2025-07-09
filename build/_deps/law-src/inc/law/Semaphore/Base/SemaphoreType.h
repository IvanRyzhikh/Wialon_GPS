#pragma once

#include <cstdint>

namespace law::sem::detail {
    enum class SemaphoreType {
        NAMED = 0,
        UNNAMED = 1
    };
}