#pragma once

#include "Vector.hpp"

namespace nate::Modules
{
    template <typename T = float>
    class Vector3 : public Vector<3, T>
    {
        Vector3() = default;

        Vector3(std::array<T, 3> init)
            : Vector<3, T>(init)
        {
        }

        template <typename IT>
        Vector3(IT begin, IT end)
            : Vector<3, T>(begin, end)
        {
        }

        Vector3(const Vector3& other) = default;
        Vector3(Vector3&& other)      = default;

        Vector3& operator=(const Vector3& other) = default;
        Vector3& operator=(Vector3&& other)      = default;

        T    X() const { return Vector<3, T>::at(0); }
        void X(T val) { Vector<3, T>::at(0) = val; }

        T    Y() const { return Vector<3, T>::at(1); }
        void Y(T val) { Vector<3, T>::at(1) = val; }

        T    Z() const { return Vector<3, T>::at(2); }
        void Z(T val) { Vector<3, T>::at(2) = val; }

        Vector3 cross(const Vector3& other) const
        {
            Vector3 rslt;
            rslt.x(Y() * other.Z()) - (Z() * other.Y());
            rslt.y((Z() * other.X()) - (X() * other.Z()));
            rslt.z((X() * other.Y()) - (Y() * other.X()));

            return rslt;
        }
    };
} // namespace nate::Modules