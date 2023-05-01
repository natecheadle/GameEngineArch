#pragma once

#include "Vector.hpp"
#include "Vector3.hpp"

namespace nate::Modules
{
    template <typename T = float>
    class Vector4 : public Vector<4, T>
    {
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
            : Vector<4, T>({other.X(), other.Y(), other.Z(), 1})
        {
        }

        Vector4(const Vector4& other) = default;
        Vector4(Vector4&& other)      = default;

        Vector4& operator=(const Vector4& other) = default;
        Vector4& operator=(Vector4&& other)      = default;

        T    X() const { return Vector<4, T>::at(0); }
        void X(T val) { Vector<3, T>::at(0) = val; }

        T    Y() const { return Vector<4, T>::at(1); }
        void Y(T val) { Vector<3, T>::at(1) = val; }

        T    Z() const { return Vector<4, T>::at(2); }
        void Z(T val) { Vector<3, T>::at(2) = val; }

        T    W() const { return Vector<4, T>::at(3); }
        void W(T val) { Vector<3, T>::at(3) = val; }

        Vector3<T> Vector3() const { return Vector3({X(), Y(), Z()}); }
    };
} // namespace nate::Modules