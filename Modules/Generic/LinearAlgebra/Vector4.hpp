#pragma once

#include "Vector.hpp"

namespace Ignosi::Modules
{
    template <typename T>
    class Vector4 : public Vector<4, T>
    {
      public:
        Vector4() noexcept = default;

        Vector4(T val)
            : Vector<4, T>(val)
        {
        }

        Vector4(std::initializer_list<T> vals) noexcept
            : Vector<4, T>(std::move(vals))
        {
        }

        Vector4(T x, T y, T z, T w)
            : Vector<4, T>({x, y, z, w})
        {
        }

        Vector4(const Vector<2, T>& other, T z = 1, T w = 1)
            : Vector<4, T>({other[0], other[1], z, w})
        {
        }

        Vector4(const Vector<3, T>& other, T w = 1)
            : Vector<4, T>({other[0], other[1], other[2], w})
        {
        }

        Vector4(const Vector4& other) noexcept = default;
        Vector4(Vector4&& other) noexcept      = default;

        Vector4& operator=(const Vector4& other) noexcept = default;
        Vector4& operator=(Vector4&& other) noexcept      = default;

        T    x() const { return Vector<4, T>::at(0); }
        void x(T val) { Vector<3, T>::at(0) = val; }

        T    y() const { return Vector<4, T>::at(1); }
        void y(T val) { Vector<3, T>::at(1) = val; }

        T    z() const { return Vector<4, T>::at(2); }
        void z(T val) { Vector<3, T>::at(2) = val; }

        T    w() const { return Vector<4, T>::at(3); }
        void w(T val) { Vector<3, T>::at(3) = val; }

        Vector3<T> ToVector3() const { return Vector3<T>(x(), y(), z()); }
    };
} // namespace Ignosi::Modules