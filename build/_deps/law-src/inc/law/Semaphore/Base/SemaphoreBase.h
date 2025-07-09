#pragma once

#include <ctime>
#include <chrono>
#include <cstdint>
#include <type_traits>
#include <semaphore.h>

#include <etl/to_string.h>
#include <boost/assert.hpp>

#include "law/Utils.h"
#include "SemaphoreType.h"
#include "law/File/File.h"
#include "law/RetCode/RetCode.h"
#include "law/Semaphore/OpenOption.h"
#include "law/Semaphore/SemaphoreTypes.h"
#include "law/Semaphore/SemaphoreSharedType.h"

namespace law::sem::detail {
    template <SemaphoreType type>
    class SemaphoreBase {
    private:
        sem_t *pSem = SEM_FAILED;

    public:
        SemaphoreBase(
                std::uint32_t initialValue = 0,
                SemaphoreSharedType sharedType = SemaphoreSharedType::LOCAL)
        requires (type == SemaphoreType::UNNAMED) {
            if (sem_init(pSem, int(sharedType), initialValue) == -1) {
                throwErrno();
            }
        }

        SemaphoreBase() noexcept
        requires (type == SemaphoreType::NAMED) = default;

        SemaphoreBase(
                std::string_view name,
                std::uint32_t initialValue = 0,
                const OpenOption &opt = Create(),
                const OpenMode &mode = OpenMode(std::uint32_t(0666)))
        requires (type == SemaphoreType::NAMED) {
            if (!init(name, initialValue, opt, mode)) {
                throwErrno();
            }
        }

        ~SemaphoreBase() noexcept {
            if constexpr (type == SemaphoreType::UNNAMED) {
                if (pSem != SEM_FAILED) {
                    BOOST_VERIFY(sem_destroy(pSem) != -1);
                }
            } else {
                if (pSem != SEM_FAILED) {
                    BOOST_VERIFY(sem_close(pSem) != -1);
                }
            }
        }

        template <SemaphoreType helper = type>
        std::enable_if_t<type == SemaphoreType::NAMED, Status>
        init(
                std::string_view name,
                std::uint32_t initialValue = 0,
                const OpenOption &opt = Create(),
                const OpenMode &mode = OpenMode(std::uint32_t(0666))
        ) noexcept {
            pSem = sem_open(name.data(), opt.getAsIntegral(), mode.getAsIntegral(), initialValue);
            if (pSem == SEM_FAILED) {
                return fromErrno();
            }

            return {};
        }

        Status post() noexcept {
            return LAW_INTERNAL_ERROR_CHECK(sem_post(pSem));
        }

        Status wait() noexcept {
            return LAW_INTERNAL_ERROR_CHECK(sem_wait(pSem));
        }

        Status wait(std::chrono::nanoseconds duration) noexcept {
            const auto secondPartOfDuration = std::chrono::duration_cast<std::chrono::seconds>(duration);
            const auto nanosecondsPartOfDuration =
                    (duration - std::chrono::duration_cast<std::chrono::nanoseconds>(secondPartOfDuration));

            timespec ts{};
            if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
                return fromErrno();
            }

            ts.tv_sec += static_cast<long>(secondPartOfDuration.count());
            ts.tv_nsec += static_cast<long>(nanosecondsPartOfDuration.count());

            return LAW_INTERNAL_ERROR_CHECK(sem_timedwait(pSem, &ts));
        }

        Status tryWait() noexcept {
            return LAW_INTERNAL_ERROR_CHECK(sem_trywait(pSem));
        }

        SemValueRet get() noexcept {
            SemValue ret;
            if (sem_getvalue(pSem, &ret) == -1) {
                return fromErrno<SemValue>();
            }

            return ret;
        }

        template <SemaphoreType helper = type>
        std::enable_if_t<type == SemaphoreType::NAMED, Status>
        remove(std::string_view path) noexcept {
            return LAW_INTERNAL_ERROR_CHECK(sem_unlink(path.data()));
        }
    };
}
