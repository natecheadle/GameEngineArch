#pragma once

#include "Vector.hpp"

namespace Ignosi::Modules
{
    template <typename T>
    class Vector2 : public Vector<2, T>
    {
        using BASE = Vector<2, T>;

      public:
        constexpr Vector2() noexcept = default;

        constexpr Vector2(const BASE& other)
            : BASE(other)
        {
        }

        constexpr Vector2(T x, T y) noexcept
            : BASE({x, y})
        {
        }

        constexpr Vector2(T val)
            : BASE(val)
        {
        }

        constexpr Vector2(std::initializer_list<T> vals) noexcept
            : BASE(std::move(vals))
        {
        }

        Vector2(const Vector2& other) noexcept = default;
        Vector2(Vector2&& other) noexcept      = default;

        Vector2& operator=(const Vector2& other) noexcept = default;
        Vector2& operator=(Vector2&& other) noexcept      = default;

        constexpr T    x() const { return Vector<2, T>::at(0U); }
        constexpr void x(T val) { Vector<2, T>::at(0U) = val; }

        constexpr T    y() const { return Vector<2, T>::at(1U); }
        constexpr void y(T val) { Vector<2, T>::at(1U) = val; }

        Vector2<T> perp() const
        {
            Vector2<T> perp(T(1), T(1));
            if (x() != T(0))
            {
                perp[0] = -y() / x();
            }
            else
            {
                perp[1] = -x() / y();
            }

            return perp;
        }
    };
} // namespace Ignosi::Modules