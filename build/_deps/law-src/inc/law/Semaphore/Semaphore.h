#pragma once

#include "Base/SemaphoreBase.h"

namespace law::sem {
    using Sempahore = detail::SemaphoreBase<detail::SemaphoreType::UNNAMED>;

    using NamedSemaphore = detail::SemaphoreBase<detail::SemaphoreType::NAMED>;
}
