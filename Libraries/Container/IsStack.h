#pragma once

#include <concepts>
#include <cstddef>
#include <type_traits>

namespace Ignosi::Libraries::Containers
{
    template <typename StackT, typename DataT>
    concept IsStack = std::movable<StackT> && requires(StackT v) {
        v.push(std::declval<const DataT&>());
        v.push(std::declval<DataT&&>());
        v.clear();
        {
            v.pop()
        } -> std::same_as<DataT>;
        {
            v.size()
        } -> std::convertible_to<size_t>;
        {
            v.empty()
        } -> std::convertible_to<bool>;
    };

} // namespace Ignosi::Libraries::Containers
