#pragma once

#include "Vector.hpp"
#include "Vector3.hpp"

namespace nate::Modules
{
    template <typename T = float>
    class Vector4 : public Vector<4, T>
    {
      public:
        Vector4() = default;

        Vector4(std::array<T, 4> init)
            : Vector<4, T>(init)
        {
        }

        template <typename IT>
        Vector4(IT begin, IT end)
            : Vector<3, T>(begin, end)
        {
        }

        Vector4(const Vector3<T>& other)
            : Vector<4, T>({other.x(), other.y(), other.z(), 1})
        {
        }

        Vector4(const Vector4& other) = default;
        Vector4(Vector4&& other)      = default;

        Vector4& operator=(const Vector4& other) = default;
        Vector4& operator=(Vector4&& other)      = default;

        T    x() const { return Vector<4, T>::at(0); }
        void x(T val) { Vector<3, T>::at(0) = val; }

        T    y() const { return Vector<4, T>::at(1); }
        void y(T val) { Vector<3, T>::at(1) = val; }

        T    z() const { return Vector<4, T>::at(2); }
        void z(T val) { Vector<3, T>::at(2) = val; }

        T    w() const { return Vector<4, T>::at(3); }
        void w(T val) { Vector<3, T>::at(3) = val; }

        Vector3<T> ToVector3() const { return Vector3<T>({x(), y(), z()}); }
    };
} // namespace nate::Modules