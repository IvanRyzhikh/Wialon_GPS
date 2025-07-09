#pragma once

#include <sys/mman.h>

#include <span>
#include <etl/string.h>

#include "law/Types.h"
#include "law/SharedMemory/OpenOption.h"

namespace law::shm {
    class SharedMemory {
    private:
        char *pShm = static_cast<char *>(MAP_FAILED);
        std::size_t shmSize;

    public:
        SharedMemory() noexcept = default;

        SharedMemory(
                std::string_view name,
                std::size_t size,
                const OpenOption &opt = (ReadWrite() | Create()),
                const OpenMode &mode = OpenMode(std::uint32_t(0666)));

        Status init(std::string_view name,
                    std::size_t size,
                    const OpenOption &opt = (ReadWrite() | Create()),
                    const OpenMode &mode = OpenMode(std::uint32_t(0666))) noexcept;

        std::span<char> get() noexcept;

        Status remove(std::string_view name) noexcept;
    };
}
