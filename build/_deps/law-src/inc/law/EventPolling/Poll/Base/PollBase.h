#pragma once

#include <map>
#include <list>
#include <ranges>
#include <chrono>
#include <vector>
#include <algorithm>

#include <etl/map.h>
#include <sys/poll.h>
#include <etl/list.h>
#include <etl/vector.h>
#include <boost/assert.hpp>

#include "law/EventPolling/Poll/IPoll.h"

#include "law/Types.h"
#include "law/RetCode/RetCode.h"
#include "law/FileDescriptor/FileDescriptor.h"

namespace law::ep::poll::detail {
    template <typename T>
    concept Container  = requires(T a, const T b) {
        requires std::regular<T>;
        requires std::swappable<T>;
        requires std::destructible<typename T::value_type>;
        requires std::same_as<typename T::reference, typename T::value_type &>;
        requires std::same_as<typename T::const_reference, const typename T::value_type &>;
        requires std::forward_iterator<typename T::iterator>;
        requires std::forward_iterator<typename T::const_iterator>;
        requires std::signed_integral<typename T::difference_type>;
        requires std::same_as<
                typename T::difference_type,
                typename std::iterator_traits<typename T::iterator>::difference_type>;
        requires std::same_as<
                typename T::difference_type,
                typename std::iterator_traits<typename T::const_iterator>::difference_type>;

        { a.begin() } -> std::same_as<typename T::iterator>;
        { a.end() } -> std::same_as<typename T::iterator>;
        { b.begin() } -> std::same_as<typename T::const_iterator>;
        { b.end() } -> std::same_as<typename T::const_iterator>;
        { a.cbegin() } -> std::same_as<typename T::const_iterator>;
        { a.cend() } -> std::same_as<typename T::const_iterator>;
        { a.size() } -> std::same_as<typename T::size_type>;
        { a.max_size() } -> std::same_as<typename T::size_type>;
        { a.empty() } -> std::same_as<bool>;
    };

    template <typename T>
    concept SubsMap = requires {
        typename T::key_type;
        typename T::mapped_type;

        requires Container<T>;
        requires std::same_as<int, typename T::key_type>;
        requires std::same_as<law::ep::poll::EventSubscriberObsPtr, typename T::mapped_type>;
    };

    template <typename T, bool cond = law::detail::IsStaticContainer<T>>
    struct PollfdVectorTypeSelector {
        using Type = std::vector<pollfd>;
    };

    template <typename T>
    struct PollfdVectorTypeSelector<T, true> {
        using Type = etl::vector<pollfd, T::MAX_SIZE>;
    };

    template <typename T>
    using PollfdVectorTypeSelectorT = typename PollfdVectorTypeSelector<T>::Type;

    template <typename T, bool cond = law::detail::IsStaticContainer<T>>
    struct TmpListTypeSelector {
        using Type = std::list<EventSubscriberObsPtr>;
    };

    template <typename T>
    struct TmpListTypeSelector<T, true> {
        using Type = etl::list<EventSubscriberObsPtr, T::MAX_SIZE>;
    };

    template <typename T>
    using TmpListTypeSelectorT = typename TmpListTypeSelector<T>::Type;

    template <SubsMap SubsMapType>
    class PollBase final : public IPoll {
    private:
        using PollfdVectorType = PollfdVectorTypeSelectorT<SubsMapType>;
        using TmpListType = TmpListTypeSelectorT<SubsMapType>;

    private:
        SubsMapType eventSubs;
        PollfdVectorType pfdVector;

        TmpListType addList;
        TmpListType removeList;

        bool onEventHandling = false;

    public:
        Status subscribe(EventSubscriberObsPtr sub) noexcept override {
            if (sub->fileDescriptor() == -1) {
                return std::unexpected(RetCode::INVALID_FILE_DESCRIPTOR);
            }

            if constexpr (law::detail::IsStaticContainer<TmpListType>) {
                if (addList.full()) {
                    return std::unexpected(RetCode::NO_AVAILABLE_SPACE);
                }
            }

            if (!onEventHandling) {
                return addToPolling(sub);
            }

            addChangesToLists(addList, removeList, sub);
            return {};
        }

        Status unsubscribe(EventSubscriberObsPtr sub) noexcept override {
            if constexpr (law::detail::IsStaticContainer<TmpListType>) {
                if (removeList.full()) {
                    return std::unexpected(RetCode::NO_AVAILABLE_SPACE);
                }
            }

            if (!onEventHandling) {
                removeFromPolling(sub);
            } else {
                addChangesToLists(removeList, addList, sub);
            }

            return {};
        }

        Status poll() noexcept override {
            return poll(std::chrono::milliseconds::max());
        }

        Status poll(std::chrono::milliseconds ms) noexcept override {
            return applyChanges()
                    .and_then(
                            [this, ms] noexcept {
                                onEventHandling = true;
                                return pollEvents((ms == std::chrono::milliseconds::max()) ? -1 : ms.count());
                            }
                    )
                    .and_then(
            [this] noexcept {
                    onEventHandling = false;
            }
            );
        }

    private:
        static void
        addChangesToLists(TmpListType &addTo, TmpListType &extractFrom, EventSubscriberObsPtr sub) noexcept {
            BOOST_ASSERT_MSG(std::addressof(addTo) != std::addressof(extractFrom), "list should be different object");
            if (std::ranges::find(addTo, sub) == addTo.end()) {
                auto it = std::ranges::find(extractFrom, sub);
                if (it != extractFrom.end()) {
                    addTo.splice(addTo.end(), extractFrom, it);
                } else {
                    addTo.push_back(sub);
                }
            }
        }

        Status applyChanges() noexcept {
            for (auto it: addList) {
                BOOST_LEAF_CHECK(addToPolling(it));
            }

            for (auto it: removeList) {
                removeFromPolling(it);
            }

            addList.clear();
            removeList.clear();

            return {};
        }

        Status addToPolling(EventSubscriberObsPtr sub) noexcept {
            if constexpr (law::detail::IsStaticContainer<SubsMapType>) {
                if (eventSubs.full()) {
                    return std::unexpected(RetCode::NO_AVAILABLE_SPACE);
                }
            }

            const auto fd = sub->fileDescriptor();
            const auto events = sub->awaitableEvents().getAsIntegral();

            const auto it = std::ranges::find(eventSubs, fd, &SubsMapType::value_type::first);
            if (it == eventSubs.cend()) {
                eventSubs[fd] = sub;
                pfdVector.push_back({fd, events, 0});
            } else {
                findPfdEntryByFd(fd)->events = events;
            }

            return {};
        }

        void removeFromPolling(EventSubscriberObsPtr sub) noexcept {
            const auto it = std::ranges::find(eventSubs, sub, &SubsMapType::value_type::second);
            if (it != eventSubs.cend()) {
                pfdVector.erase(findPfdEntryByFd(it->first));
                eventSubs.erase(it);
            }
        }

        Status pollEvents(int ms) noexcept {
            switch (::poll(pfdVector.data(), pfdVector.size(), ms)) {
                case -1:
                    return ((errno != EINTR) ? fromErrno() : pollEvents(ms));
                case 0:
                    return std::unexpected(RetCode::TIMEOUT);
                default:
                    handleIncomingEvents();
                    clearIncomingEvents();
                    return {};
            }
        }

        auto findPfdEntryByFd(int fd) noexcept {
            return std::ranges::find(pfdVector, fd, &pollfd::fd);
        }

        void handleIncomingEvents() {
            for (const auto &it: pfdVector) {
                if (it.revents == 0) {
                    continue;
                }

                auto &sub = eventSubs[it.fd];
                if (it.revents & POLLNVAL) {
                    sub->onDelete();
                    removeFromPolling(sub);
                }

                if (it.revents & (POLLERR | POLLHUP | POLLRDHUP)) {
                    sub->onError(Error(it.revents));
                    continue;
                }

                if (it.revents & (POLLIN | POLLPRI | POLLOUT | POLLRDNORM | POLLRDBAND | POLLWRNORM | POLLWRBAND)) {
                    sub->onEvent(Event(it.revents));
                }
            }
        }

        void clearIncomingEvents() noexcept {
            for (auto &it: pfdVector) {
                it.revents = 0;
            }
        }
    };
}
