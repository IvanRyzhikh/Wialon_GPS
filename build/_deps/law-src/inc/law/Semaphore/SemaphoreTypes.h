#pragma once

#include "law/Types.h"
#include "law/RetCode/RetCode.h"

namespace law::sem {
    using SemValue = int;
    using SemValueRet = CustomError<SemValue>;
}
