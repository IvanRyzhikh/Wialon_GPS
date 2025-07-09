#pragma once

#include <type_traits>

#include <etl/vector.h>
#include <boost/container/small_vector.hpp>

#include "J1939Types.h"

namespace law::socket::can {
    template <std::size_t filtersMaxCount = std::dynamic_extent>
    class J1939FiltersBuilder final {
    private:
        static constexpr std::size_t kOptimalFiltersCount = 32;

    private:
        using FilterSet = std::conditional_t<
            (filtersMaxCount == std::dynamic_extent),
            boost::container::small_vector<J1939Filter, kOptimalFiltersCount>,
            etl::vector<J1939Filter, filtersMaxCount>
        >;

    protected:
        class J1939FilterBuilderBase {
        private:
            J1939FiltersBuilder &rJ1939FiltersBuilder;

        public:
            J1939FilterBuilderBase(
                J1939FiltersBuilder &j1939FiltersBuilderRef) noexcept: rJ1939FiltersBuilder(j1939FiltersBuilderRef) {}

            virtual J1939FilterBuilderBase &setName(name_t) noexcept {
                return *this;
            }

            virtual J1939FilterBuilderBase &setName(name_t, name_t) noexcept {
                return *this;
            }

            virtual J1939FilterBuilderBase &setPgn(pgn_t) noexcept {
                return *this;
            }

            virtual J1939FilterBuilderBase &setPgn(pgn_t, pgn_t) noexcept {
                return *this;
            }

            virtual J1939FilterBuilderBase &setAddr(__u8) noexcept {
                return *this;
            }

            virtual J1939FilterBuilderBase &setAddr(__u8, __u8) noexcept {
                return *this;
            }

            J1939FiltersBuilder &apply() noexcept {
                return rJ1939FiltersBuilder;
            }
        };

        class J1939FilterBuilder final : public J1939FilterBuilderBase {
        private:
            j1939_filter &rFilter;

        public:
            J1939FilterBuilder(
                J1939FiltersBuilder &j1939FiltersBuilderRef,
                j1939_filter &filterRef
            ) noexcept: J1939FilterBuilderBase(j1939FiltersBuilderRef), rFilter(filterRef) {
            }

            J1939FilterBuilderBase &setName(name_t name) noexcept override {
                rFilter.name = name;
                return *this;
            }

            J1939FilterBuilderBase &setName(name_t name, name_t mask) noexcept override {
                rFilter.name = name;
                rFilter.name_mask = mask;
                return *this;
            }

            J1939FilterBuilderBase &setPgn(pgn_t pgn) noexcept override {
                rFilter.pgn = pgn;
                return *this;
            }

            J1939FilterBuilderBase &setPgn(pgn_t pgn, pgn_t mask) noexcept override {
                rFilter.pgn = pgn;
                rFilter.pgn_mask = mask;
                return *this;
            }

            J1939FilterBuilderBase &setAddr(__u8 addr) noexcept override {
                rFilter.addr = addr;
                return *this;
            }

            J1939FilterBuilderBase &setAddr(__u8 addr, __u8 mask) noexcept override {
                rFilter.addr = addr;
                rFilter.addr_mask = mask;
                return *this;
            }
        };

    private:
        FilterSet filters;

    public:
        J1939FilterBuilderBase create() noexcept {
            if constexpr (requires { FilterSet::MAX_SIZE; }) {
                if (filters.full()) {
                    return {*this};
                }
            }

            auto &filter = filters.emplace_back(
                J1939_NO_NAME, UINT64_MAX, J1939_NO_PGN, UINT32_MAX, J1939_NO_ADDR, UINT8_MAX
            );
            return J1939FilterBuilder(*this, filter);
        }

        [[nodiscard]] J1939FilterSequence get() const noexcept {
            return {filters.data(), filters.size()};
        }
    };
}
