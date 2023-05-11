#pragma once

#include "Angle.hpp"

namespace nate::Modules
{
    template <class T>
    class Radian : public Angle<T, Radian<T>>
    {
        friend Angle<T, Radian<T>>;

      public:
        Radian() = default;
        Radian(float val)
            : Angle<T, Radian>(val)
        {
        }

        virtual ~Radian() = default;

        template <class DERIVED_OTHER>
        Radian(const Angle<T, DERIVED_OTHER>& other)
            : Angle<T, Radian<T>>(other.Radians())
        {
        }

      protected:
        float ValPerRad() const override { return 1.0; };
    };
} // namespace nate::Modules
