#pragma once

#include "AngularVelocity.hpp"
#include "Radian.hpp"
#include "Second.hpp"
#include "Unit.hpp"

namespace Ignosi::Libraries::Math
{
    template <class T, class AngleUnit, class VelTimeUnit, class AccelTimeUnit>
    class AngularAcceleration : public Unit<T, UnitType::AngularAcceleration, AngularAcceleration<T, AngleUnit, VelTimeUnit, AccelTimeUnit>>
    {
        using BASE = Unit<T, UnitType::AngularAcceleration, AngularAcceleration<T, AngleUnit, VelTimeUnit, AccelTimeUnit>>;
        static constexpr AngleUnit     angle;
        static constexpr VelTimeUnit   velTime;
        static constexpr AccelTimeUnit accelTime;

      public:
        constexpr AngularAcceleration(T val)
            : BASE(val / PerBase())
        {
        }

        AngularAcceleration()                                     = default;
        AngularAcceleration(const AngularAcceleration& other)     = default;
        AngularAcceleration(AngularAcceleration&& other) noexcept = default;
        virtual ~AngularAcceleration()                            = default;

        AngularAcceleration& operator=(const AngularAcceleration& other) = default;
        AngularAcceleration& operator=(AngularAcceleration&& other)      = default;

        template <class OtherAngleUnit, class OtherVelTimeUnit, class OtherAccelTimeUnit>
        AngularAcceleration(const AngularAcceleration<T, OtherAngleUnit, OtherVelTimeUnit, OtherAccelTimeUnit>& other)
            : BASE(other.BaseValue())
        {
        }

        template <class OtherAngleUnit, class OtherVelTimeUnit, class OtherAccelTimeUnit>
        AngularAcceleration& operator=(const AngularAcceleration<T, OtherAngleUnit, OtherVelTimeUnit, OtherAccelTimeUnit>& other)
        {
            if (this == &other)
                return *this;

            BASE::BaseValue(other.BaseValue());
            return *this;
        }

        template <class OtherTimeUnit>
        AngularVelocity<T, AngleUnit, VelTimeUnit> operator*(const Time<T, OtherTimeUnit>& other)
        {
            AngularVelocity<T, AngleUnit, VelTimeUnit> rslt;
            rslt.Value(rslt.FromBase(BASE::BaseValue() * other.BaseValue()));
            return rslt;
        }

        template <class OtherAngleUnit, class OtherVelTimeUnit, class OtherAccelTimeUnit>
        friend AngularAcceleration operator/(
            const AngularVelocity<T, OtherAngleUnit, OtherVelTimeUnit>& lhs,
            const Time<T, OtherAccelTimeUnit>&                          rhs)
        {
            AngularAcceleration rslt;
            rslt.BaseValue(lhs.BaseValue() / rhs.BaseValue());
            return rslt;
        }

        constexpr T PerBase() const override { return angle.PerBase() / (velTime.PerBase() * accelTime.PerBase()); }
    };

    template <class T>
    using RadianPerSecondPerSecond = AngularAcceleration<T, Radian<T>, Second<T>, Second<T>>;
} // namespace Ignosi::Libraries::Math
