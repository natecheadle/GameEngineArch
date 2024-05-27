#pragma once

#include <tuple>

namespace Ignosi::Libraries
{
    namespace
    {
        template <typename T, typename Tuple>
        struct TupleElementIndexHelper;

        template <typename T>
        struct TupleElementIndexHelper<T, std::tuple<>>
        {
            static constexpr std::size_t value = 0;
        };

        template <typename T, typename... Rest>
        struct TupleElementIndexHelper<T, std::tuple<T, Rest...>>
        {
            static constexpr std::size_t value = 0;
            using RestTuple                    = std::tuple<Rest...>;
            static_assert(
                TupleElementIndexHelper<T, RestTuple>::value == std::tuple_size_v<RestTuple>,
                "type appears more than once in tuple");
        };

        template <typename T, typename First, typename... Rest>
        struct TupleElementIndexHelper<T, std::tuple<First, Rest...>>
        {
            using RestTuple                    = std::tuple<Rest...>;
            static constexpr std::size_t value = 1 + TupleElementIndexHelper<T, RestTuple>::value;
        };
    } // namespace

    template <typename T, typename Tuple>
    struct TupleElementIndex
    {
        static constexpr std::size_t value = TupleElementIndexHelper<T, Tuple>::value;
        static_assert(value < std::tuple_size_v<Tuple>, "type does not appear in tuple");
    };

    template <typename T, typename Tuple>
    inline constexpr std::size_t TupleElementIndex_v = TupleElementIndex<T, Tuple>::value;
} // namespace Ignosi::Libraries
