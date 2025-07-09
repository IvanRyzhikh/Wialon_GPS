#pragma once

#include <errcode/errcode.h>

enum class ret_code {
    GENERIC,
    BUSY,
    OK
};

using Status = std::expected<void, ret_code>;

template<typename T>
using CustomError = std::expected<T, ret_code>;
