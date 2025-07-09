#include <unistd.h>
#include <sys/mman.h>

#include "law/Utils.h"
#include "law/SharedMemory/SharedMemory.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::shm {
    SharedMemory::SharedMemory(std::string_view name, std::size_t size, const OpenOption &opt, const OpenMode &mode) {
        if (!init(name, size, opt, mode)) {
            throwErrno();
        }
    }

    Status SharedMemory::init(
            std::string_view name, std::size_t size, const OpenOption &opt, const OpenMode &mode) noexcept {
        fd::FileDescriptor fd(shm_open(name.data(), opt.getAsIntegral(), mode.getAsIntegral()));
        if (!fd) {
            return fromErrno();
        }

        if ((opt & Create()) && (ftruncate(fd.getNativeHandler(), size) == -1)) {
            return fromErrno();
        }

        if (opt & ReadWrite()) {
            pShm = reinterpret_cast<char *>(mmap(nullptr, size, (PROT_READ | PROT_WRITE), MAP_SHARED, fd.getNativeHandler(), 0));
        } else {
            pShm = reinterpret_cast<char *>(mmap(nullptr, size, PROT_READ, MAP_SHARED, fd.getNativeHandler(), 0));
        }

        if (pShm == MAP_FAILED) {
            return fromErrno();
        }

        shmSize = size;
        return {};
    }

    std::span<char> SharedMemory::get() noexcept {
        return {pShm, shmSize};
    }

    Status SharedMemory::remove(std::string_view name) noexcept {
        return LAW_INTERNAL_ERROR_CHECK(shm_unlink(name.data()));
    }
}
