#pragma once

#include "Vector.hpp"

#include <array>

namespace nate::Modules
{
    template <typename T>
    class Vector3 : public Vector<3, T>
    {
        using BASE = Vector<3, T>;

      public:
        Vector3() noexcept = default;
        Vector3(T x, T y, T z) noexcept
            : BASE({x, y, z})
        {
        }

        Vector3(const Vector<3, T>& other) noexcept
            : BASE(other)
        {
        }

        Vector3(Vector<3, T>&& other) noexcept
            : BASE(other)
        {
        }

        Vector3(const Vector3& other) noexcept = default;
        Vector3(Vector3&& other) noexcept      = default;

        Vector3& operator=(const Vector3& other) noexcept = default;
        Vector3& operator=(Vector3&& other) noexcept      = default;

        T    x() const { return BASE::at(0); }
        void x(T val) { BASE::at(0) = val; }

        T    y() const { return BASE::at(1); }
        void y(T val) { BASE::at(1) = val; }

        T    z() const { return BASE::at(2); }
        void z(T val) { BASE::at(2) = val; }

        T length() const { return x() * x() + y() * y() + z() * z(); }

        Vector3 cross(const Vector3& other) const
        {
            Vector3 rslt;
            rslt.x((y() * other.z()) - (z() * other.y()));
            rslt.y((z() * other.x()) - (x() * other.z()));
            rslt.z((x() * other.y()) - (y() * other.x()));

            return rslt;
        }

        Vector3 normalize() const
        {
            Vector3 rslt(*this);
            return rslt / BASE::magnitude();
        }

        Vector3& normalize_this()
        {
            *this /= BASE::magnitude();
            return *this;
        }
    };
} // namespace nate::Modules