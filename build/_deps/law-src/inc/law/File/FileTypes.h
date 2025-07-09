#pragma once

#include "law/Types.h"
#include "law/RetCode/RetCode.h"

namespace law::file {
    using FileSize = std::size_t;
    using FileSizeRet = CustomError<FileSize>;

    using FileModes = OpenMode;
    using FileModesRet = CustomError<FileModes>;
}
