#pragma once

#include "Angle.hpp"

namespace Ignosi::Libraries::Math
{
    template <class T>
    class Radian : public Angle<T, "rad", Radian<T>>
    {
        using BASE = Angle<T, "rad", Radian<T>>;

      public:
        constexpr Radian(T val)
            : BASE(val)
        {
        }

        constexpr Radian()              = default;
        Radian(const Radian& other)     = default;
        Radian(Radian&& other) noexcept = default;
        virtual ~Radian()               = default;

        Radian& operator=(const Radian& other) = default;
        Radian& operator=(Radian&& other)      = default;

        template <class OtherDerived, StringLiteral OtherUnits>
        Radian(const Angle<T, OtherUnits, OtherDerived>& other)
            : BASE(0)
        {
            BASE::BASE::BaseValue(other.BaseValue());
        }

        template <class OtherDerived, StringLiteral OtherUnits>
        Radian& operator=(const Angle<T, OtherUnits, OtherDerived>& other)
        {
            BASE::BASE::BaseValue(other.BaseValue());
            return *this;
        }

        constexpr T PerBase() const override { return static_cast<T>(1.0); }
    };
} // namespace Ignosi::Libraries::Math
