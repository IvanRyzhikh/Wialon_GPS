#include <fcntl.h>

#include "law/Types.h"

namespace law {
    OpenMode OpenMode::operator&(const OpenMode &of) const noexcept {
        return OpenMode{getAsIntegral() & of.getAsIntegral()};
    }

    OpenMode OpenMode::operator|(const OpenMode &of) const noexcept {
        return OpenMode{getAsIntegral() | of.getAsIntegral()};
    }

    OwnerReadWriteExec::OwnerReadWriteExec() noexcept: OpenMode(S_IRWXU) { }

    OwnerRead::OwnerRead() noexcept: OpenMode(S_IRUSR) { }

    OwnerWrite::OwnerWrite() noexcept: OpenMode(S_IWUSR) { }

    OwnerExec::OwnerExec() noexcept: OpenMode(S_IXUSR) { }

    GroupReadWriteExec::GroupReadWriteExec() noexcept: OpenMode(S_IRWXG) { }

    GroupRead::GroupRead() noexcept: OpenMode(S_IRGRP) { }

    GroupWrite::GroupWrite() noexcept: OpenMode(S_IWGRP) { }

    GroupExec::GroupExec() noexcept: OpenMode(S_IXGRP) { }

    OtherReadWriteExec::OtherReadWriteExec() noexcept: OpenMode(S_IRWXO) { }

    OtherRead::OtherRead() noexcept: OpenMode(S_IROTH) { }

    OtherWrite::OtherWrite() noexcept: OpenMode(S_IWOTH) { }

    OtherExec::OtherExec() noexcept: OpenMode(S_IXOTH) { }
}
