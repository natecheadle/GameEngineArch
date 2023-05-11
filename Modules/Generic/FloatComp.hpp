#pragma once

#include <algorithm>
#include <cmath>
#include <type_traits>

namespace nate::Modules
{
    template <typename T, typename... Args>
    T max(T a, T b, Args... args)
    {
        return std::max(std::max(a, b), args...);
    }

    template <typename T, typename... Args>
    T min(T a, T b, Args... args)
    {
        return std::min(std::min(a, b), args...);
    }

    template <typename T>
    bool almost_eq(T lhs, T rhs)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            T maxXYOne = max<T>(1, std::abs(lhs), std::abs(rhs));
            return std::abs(lhs - rhs) <= std::numeric_limits<T>::epsilon() * maxXYOne;
        }
        return lhs == rhs;
    }
} // namespace nate::Modules