#pragma once

#include <sys/mman.h>

namespace law::file {
    enum class MappedFileVisibilityRule : int {
        SHARED = MAP_SHARED,
        PRIVATE = MAP_PRIVATE
    };
}
