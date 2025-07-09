#pragma once

#include <expected>

#define EXPECTED_TOKEN_PASTE(x, y) x ## y
#define EXPECTED_TOKEN_PASTE2(x, y) EXPECTED_TOKEN_PASTE(x, y)
#define EXPECTED_TMP EXPECTED_TOKEN_PASTE2(expected_tmp_, __LINE__)

#define EXPECTED_CHECK(r)                                       \
    ({                                                          \
        auto && EXPECTED_TMP = (r);                             \
        if (!EXPECTED_TMP) {                                    \
            return std::unexpected(EXPECTED_TMP.error());       \
        }                                                       \
                                                                \
        std::move(EXPECTED_TMP);                                \
    }).value()
#define EXPECTED_CHECK_CORO(r)                                  \
    ({                                                          \
        auto && EXPECTED_TMP = (r);                             \
        if (!EXPECTED_TMP) {                                    \
            co_return std::unexpected(EXPECTED_TMP.error());    \
        }                                                       \
                                                                \
        std::move(EXPECTED_TMP);                                \
    }).value()

#define EXPECTED_ASSIGN(v, r)                                           \
    auto && EXPECTED_TMP = r;                                           \
    if (!EXPECTED_TMP)                                                  \
        return std::unexpected(EXPECTED_TMP.error());                   \
    v = std::forward<decltype(EXPECTED_TMP)>(EXPECTED_TMP).value()
#define EXPECTED_AUTO(v, r) EXPECTED_ASSIGN(auto v, r)

#define EXPECTED_ASSIGN_CORO(v, r)                                      \
    auto && EXPECTED_TMP = r;                                           \
    if (!EXPECTED_TMP)                                                  \
        co_return std::unexpected(EXPECTED_TMP.error());                \
    v = std::forward<decltype(EXPECTED_TMP)>(EXPECTED_TMP).value()
#define EXPECTED_AUTO_CORO(v, r) EXPECTED_ASSIGN_CORO(auto v, r)
