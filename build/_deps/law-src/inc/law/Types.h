#pragma once

#include <span>
#include <net/if.h>
#include <sys/uio.h>
#include <type_traits>
#include <string_view>

#include <etl/string.h>

#include "FlagValue.h"
#include "RetCode/RetCode.h"

namespace law {
    namespace detail {
        template <typename T>
        concept IsStaticContainer = requires {
            T::MAX_SIZE;
        };
    }

    struct OpenMode : public UInt32FlagValue {
        using UInt32FlagValue::UInt32FlagValue;

        OpenMode operator&(const OpenMode &of) const noexcept;

        OpenMode operator|(const OpenMode &of) const noexcept;
    };

    struct OwnerReadWriteExec : OpenMode {
        OwnerReadWriteExec() noexcept;
    };

    struct OwnerRead : OpenMode {
        OwnerRead() noexcept;
    };

    struct OwnerWrite : OpenMode {
        OwnerWrite() noexcept;
    };

    struct OwnerExec : OpenMode {
        OwnerExec() noexcept;
    };

    struct GroupReadWriteExec : OpenMode {
        GroupReadWriteExec() noexcept;
    };

    struct GroupRead : OpenMode {
        GroupRead() noexcept;
    };

    struct GroupWrite : OpenMode {
        GroupWrite() noexcept;
    };

    struct GroupExec : OpenMode {
        GroupExec() noexcept;
    };

    struct OtherReadWriteExec : OpenMode {
        OtherReadWriteExec() noexcept;
    };

    struct OtherRead : OpenMode {
        OtherRead() noexcept;
    };

    struct OtherWrite : OpenMode {
        OtherWrite() noexcept;
    };

    struct OtherExec : OpenMode {
        OtherExec() noexcept;
    };

    template<class T>
    concept StringLike = std::is_convertible_v<T, std::string_view>;

    using Fd = int;
    using FdRet = CustomError<Fd>;

    using Bool = bool;
    using BoolRet = CustomError<Bool>;

    using Int = int;
    using IntRet = CustomError<Int>;

    using UInt32 = std::uint32_t;
    using UInt32Ret = CustomError<UInt32>;

    using Blob = std::string;
    using BlobRet = CustomError<Blob>;

    using BlobView = std::string_view;
    using BlobViewRet = CustomError<BlobView>;

    using AvailableDataSize = int;
    using AvailableDataSizeRet = CustomError<AvailableDataSize>;

    using ReadCount = std::size_t;
    using ReadCountRet = CustomError<ReadCount>;

    using WriteCount = std::size_t;
    using WriteCountRet = CustomError<WriteCount>;

    using FileSize = std::size_t;
    using FileSizeRet = CustomError<FileSize>;

    using FileModes = OpenMode;
    using FileModesRet = CustomError<FileModes>;

    using IoVectorSet = std::span<const iovec>;

    using IfaceName = etl::string<IF_NAMESIZE>;
    using IfaceNameRet = CustomError<IfaceName>;
}
