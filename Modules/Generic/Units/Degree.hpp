#pragma once

#include "Angle.hpp"

namespace Ignosi::Modules
{
    template <class T>
    class Degree : public Angle<T, Degree<T>>
    {
        friend Angle<T, Degree<T>>;

      public:
        Degree() = default;
        Degree(float val)
            : Angle<T, Degree>(val * Angle<T, Degree>::RadPerDegree())
        {
        }

        virtual ~Degree() = default;

        template <class DERIVED_OTHER>
        Degree(const Angle<T, DERIVED_OTHER>& other)
            : Degree(other.Radians() * Angle<T, Degree<T>>::DegreePerRad())
        {
        }

        float ValPerRad() const override { return Angle<T, Degree<T>>::DegreePerRad(); };
    };
} // namespace Ignosi::Modules
