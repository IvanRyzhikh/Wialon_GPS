#pragma once

#include <errcode/errcode.h>

enum class RetCode {
    UNEXPECTED,

    PARSE_ERROR,
    CRC_ERROR,
    INVALID_LENGTH
};

using Status = std::expected<void, RetCode>;

template <typename T>
using CustomError = std::expected<T, RetCode>;
