#pragma once

#include <map>
#include <vector>

#include <etl/map.h>
#include <etl/vector.h>

#include "Base/PollBase.h"

namespace law::ep::poll {
    template <std::size_t maxSubs>
    using StaticPoll =
            detail::PollBase<
                    etl::map<int, EventSubscriberObsPtr, maxSubs>
            >;

    using Poll =
            detail::PollBase<
                    std::map<int, EventSubscriberObsPtr>
            >;
}
