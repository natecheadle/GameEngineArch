#pragma once

#include "Vector.hpp"

namespace nate::Modules
{
    template <typename T>
    class Vector2 : public Vector<2, T>
    {
        using BASE = Vector<2, T>;

      public:
        Vector2() noexcept = default;

        Vector2(T x, T y)
            : BASE({x, y})
        {
        }

        Vector2(T val)
            : BASE(val)
        {
        }

        Vector2(std::initializer_list<T> vals) noexcept
            : BASE(std::move(vals))
        {
        }

        Vector2(BASE&& val)
            : BASE(std::move(val))
        {
        }

        Vector2(const Vector2& other) noexcept = default;
        Vector2(Vector2&& other) noexcept      = default;

        Vector2& operator=(const Vector2& other) noexcept = default;
        Vector2& operator=(Vector2&& other) noexcept      = default;

        T    x() const { return BASE::at(0); }
        void x(T val) { BASE::at(0) = val; }

        T    y() const { return BASE::at(1); }
        void y(T val) { BASE::at(1) = val; }
    };
} // namespace nate::Modules