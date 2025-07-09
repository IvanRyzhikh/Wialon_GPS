#include <type_traits>

#include <boost/assert.hpp>

#include "law/File/MappedFile.h"

namespace law::file {
    MappedFile::MappedFile(
            std::string_view filename, const MappedFileAccessRule &ar, MappedFileVisibilityRule vr) noexcept {
        if (!createMapping(filename, ar, vr)) {
            throwErrno();
        }
    }

    MappedFile::MappedFile(MappedFile &&other) noexcept {
        pMappedFile = other.pMappedFile;
        mappedFileSize = other.mappedFileSize;

        other.pMappedFile = nullptr;
    }

    MappedFile::~MappedFile() noexcept {
        if (pMappedFile) {
            BOOST_ASSERT(munmap(pMappedFile, mappedFileSize) == 0);
        }
    }

    MappedFile &MappedFile::operator=(MappedFile &&rhs) noexcept {
        if (&rhs != this) {
            pMappedFile = rhs.pMappedFile;
            mappedFileSize = rhs.mappedFileSize;

            rhs.pMappedFile = nullptr;
        }

        return *this;
    }

    Status MappedFile::createMapping(
            std::string_view filename, const MappedFileAccessRule &ar, MappedFileVisibilityRule vr) noexcept {
        static constexpr std::uint8_t kMmapFlagToOpenFlagNormalizer = 1;

        /**
         * mmap PROT_READ (0x01) -> open O_RDONLY (0x00)
         * mmap PROT_WRITE (0x02) -> open O_WRONLY (0x01)
         * mmap PROT_READ | PROT_WRITE (0x03) -> open O_RDWR (0x02)
         *
         * That's why we just decrement ar value by 1. Simple and fast.
         */
        File file;
        return file.open(filename, OpenOption{ar.getAsIntegral() - kMmapFlagToOpenFlagNormalizer})
                .and_then(
                        [&file] noexcept {
                            return file.getFileSize();
                        }
                )
                .and_then(
                        [this, &file, &ar, &vr](std::size_t fileSize) noexcept {
                            mappedFileSize = fileSize;

                            pMappedFile =
                                    static_cast<char *>(
                                            mmap(
                                                    nullptr,
                                                    mappedFileSize,
                                                    ar.getAsIntegral(),
                                                    std::underlying_type_t<MappedFileVisibilityRule>(vr),
                                                    file.getNativeHandler(),
                                                    0
                                            )
                                    );
                            if (pMappedFile == MAP_FAILED) {
                                pMappedFile = nullptr;
                                return fromErrno();
                            }

                            return Status();
                        }
                );
    }

    Status MappedFile::flush(const MappedFileSyncRule &sr) noexcept {
        return LAW_INTERNAL_ERROR_CHECK(msync(pMappedFile, mappedFileSize, sr.getAsIntegral()));
    }

    Status MappedFile::acceptAdvise(const MappedFileAdvice &a) noexcept {
        return LAW_INTERNAL_ERROR_CHECK(madvise(pMappedFile, mappedFileSize, a.getAsIntegral()));
    }

    std::span<char> MappedFile::get() noexcept {
        return {pMappedFile, mappedFileSize};
    }
}
