#pragma once

namespace law::sem {
    enum class SemaphoreSharedType : int {
        LOCAL = 0,
        PROCESS_SHARED
    };
}
