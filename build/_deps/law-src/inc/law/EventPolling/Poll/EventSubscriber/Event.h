#pragma once

#include "law/FlagValue.h"

namespace law::ep::poll {
    struct Event : public ShortFlagValue {
        using ShortFlagValue::ShortFlagValue;

        Event operator&(const Event &e) const noexcept;

        Event operator|(const Event &e) const noexcept;
    };
    
    struct IncomingDataAvailable final : public Event {
        IncomingDataAvailable() noexcept;
    };

    struct UrgentDataAvailable final : public Event {
        UrgentDataAvailable() noexcept;
    };

    struct AvailableForWriting final : public Event {
        AvailableForWriting() noexcept;
    };

    struct PriorityDataAvailable final : public Event {
        PriorityDataAvailable() noexcept;
    };

    struct AvailableForPriorityWriting final : public Event {
        AvailableForPriorityWriting() noexcept;
    };
}
