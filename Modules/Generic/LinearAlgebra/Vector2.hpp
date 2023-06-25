#pragma once

#include "Vector.hpp"

namespace nate::Modules
{
    template <typename T>
    class Vector2 : public Vector<2, T>
    {
      public:
        Vector2() noexcept = default;

        Vector2(T x, T y)
            : Vector<2, T>({x, y})
        {
        }

        Vector2(T val)
            : Vector<2, T>(val)
        {
        }

        Vector2(std::initializer_list<T> vals) noexcept
            : Vector<2, T>(std::move(vals))
        {
        }

        Vector2(const Vector2& other) noexcept = default;
        Vector2(Vector2&& other) noexcept      = default;

        Vector2& operator=(const Vector2& other) noexcept = default;
        Vector2& operator=(Vector2&& other) noexcept      = default;

        T    x() const { return Vector<2, T>::at(0U); }
        void x(T val) { Vector<2, T>::at(0U) = val; }

        T    y() const { return Vector<2, T>::at(1U); }
        void y(T val) { Vector<2, T>::at(1U) = val; }
    };
} // namespace nate::Modules