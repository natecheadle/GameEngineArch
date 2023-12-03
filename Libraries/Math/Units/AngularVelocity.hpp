#pragma once

#include "Radian.hpp"
#include "Second.hpp"
#include "Unit.hpp"

namespace Ignosi::Libraries::Math
{
    template <class T, StringLiteral Units, class AngleUnit, class TimeUnit>
    class AngularVelocity : public Unit<T, UnitType::AngularVelocity, Units, AngularVelocity<T, Units, AngleUnit, TimeUnit>>
    {
        using BASE                       = Unit<T, UnitType::AngularVelocity, Units, AngularVelocity<T, Units, AngleUnit, TimeUnit>>;
        static constexpr AngleUnit angle = AngleUnit(1.0);
        static constexpr TimeUnit  time  = TimeUnit(1.0);

      public:
        constexpr AngularVelocity(T val)
            : BASE(val / PerBase())
        {
        }

        AngularVelocity()                                 = default;
        AngularVelocity(const AngularVelocity& other)     = default;
        AngularVelocity(AngularVelocity&& other) noexcept = default;
        virtual ~AngularVelocity()                        = default;

        AngularVelocity& operator=(const AngularVelocity& other) = default;
        AngularVelocity& operator=(AngularVelocity&& other)      = default;

        template <class OtherAngleUnit, class OtherTimeUnit, StringLiteral OtherUnits>
        AngularVelocity(const AngularVelocity<T, OtherUnits, OtherAngleUnit, OtherTimeUnit>& other)
            : BASE(other.BaseValue())
        {
        }

        template <class OtherAngleUnit, class OtherTimeUnit, StringLiteral OtherUnits>
        AngularVelocity& operator=(const AngularVelocity<T, OtherUnits, OtherAngleUnit, OtherTimeUnit>& other)
        {
            if (this == &other)
                return *this;

            BASE::BaseValue(other.BaseValue());
            return *this;
        }

        template <class OtherTimeUnit, StringLiteral OtherUnits>
        AngleUnit operator*(const Time<T, OtherUnits, OtherTimeUnit>& other)
        {
            AngleUnit rslt;
            rslt.Value(rslt.FromBase(BASE::BaseValue() * other.BaseValue()));
            return rslt;
        }

        template <class OtherAngleUnit, class OtherTimeUnit, StringLiteral OtherUnits1, StringLiteral OtherUnits2>
        friend AngularVelocity operator/(const Angle<T, OtherUnits1, OtherAngleUnit>& lhs, const Time<T, OtherUnits2, OtherTimeUnit>& rhs)
        {
            AngularVelocity rslt;
            rslt.BaseValue(lhs.BaseValue() / rhs.BaseValue());
            return rslt;
        }

        constexpr T PerBase() const override { return angle.PerBase() / time.PerBase(); }
    };

    template <class T>
    using RadianPerSecond = AngularVelocity<T, "rad/sec", Radian<T>, Second<T>>;
} // namespace Ignosi::Libraries::Math
