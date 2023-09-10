#pragma once

#include "Angle.hpp"

namespace Ignosi::Modules
{
    template <class T>
    class Radian : public Angle<T, Radian<T>>
    {
        friend Angle<T, Radian<T>>;

      public:
        Radian() = default;
        constexpr Radian(float val)
            : Angle<T, Radian>(val)
        {
        }
        Radian(const Radian& other) = default;
        virtual ~Radian()           = default;

        template <class DERIVED_OTHER>
        Radian(const Angle<T, DERIVED_OTHER>& other)
            : Angle<T, Radian<T>>(other.Radians())
        {
        }

        float ValPerRad() const override { return 1.0; };
    };
} // namespace Ignosi::Modules
