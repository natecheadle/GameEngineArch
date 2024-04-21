#pragma once

#include <concepts>
#include <cstddef>
#include <optional>
#include <type_traits>

namespace Ignosi::Libraries::Containers
{
    template <typename RingBufferT, typename DataT>
    concept IsRingBuffer = std::movable<RingBufferT> && requires(RingBufferT v) {
        v.push(std::declval<const DataT&>());
        v.push(std::declval<DataT&&>());
        {
            v.pop()
        } -> std::same_as<std::optional<DataT>>;
        {
            v.empty()
        } -> std::convertible_to<bool>;
        {
            v.count()
        } -> std::convertible_to<size_t>;
        {
            v.size()
        } -> std::convertible_to<size_t>;
    };

} // namespace Ignosi::Libraries::Containers
