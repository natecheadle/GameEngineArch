#pragma once

#include <concepts>
#include <cstddef>
#include <type_traits>

namespace Ignosi::Libraries::Containers
{
    template <typename QueueT, typename DataT>
    concept IsQueue = std::movable<QueueT> && requires(QueueT v) {
        v.push(std::declval<const DataT&>());
        v.push(std::declval<DataT&&>());
        v.clear();
        {
            v.pop()
        } -> std::same_as<DataT>;
        {
            v.empty()
        } -> std::convertible_to<bool>;
    };

} // namespace Ignosi::Libraries::Containers
