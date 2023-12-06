#pragma once

#include "AngularVelocity.hpp"
#include "Radian.hpp"
#include "Second.hpp"
#include "Unit.hpp"

namespace Ignosi::Libraries::Math
{
    template <class T, class AngleUnit, class VelTimeUnit, class AccelTimeUnit>
    class AngularAcceleration
        : public Unit<
              T,
              UnitType::AngularAcceleration,
              AngleUnit::StringLit() + StringLiteral("/") + VelTimeUnit::StringLit() + StringLiteral("/") + AccelTimeUnit::StringLit(),
              AngularAcceleration<T, AngleUnit, VelTimeUnit, AccelTimeUnit>>
    {
        using BASE = Unit<
            T,
            UnitType::AngularAcceleration,
            AngleUnit::StringLit() + StringLiteral("/") + VelTimeUnit::StringLit() + StringLiteral("/") + AccelTimeUnit::StringLit(),
            AngularAcceleration<T, AngleUnit, VelTimeUnit, AccelTimeUnit>>;
        static constexpr AngleUnit     angle     = AngleUnit(1.0);
        static constexpr VelTimeUnit   velTime   = VelTimeUnit(1.0);
        static constexpr AccelTimeUnit accelTime = AccelTimeUnit(1.0);

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

        template <class OtherTimeUnit, StringLiteral STR_1>
        AngularVelocity<T, AngleUnit, VelTimeUnit> operator*(const Time<T, STR_1, OtherTimeUnit>& other)
        {
            AngularVelocity<T, AngleUnit, VelTimeUnit> rslt;
            rslt.Value(rslt.FromBase(BASE::BaseValue() * other.BaseValue()));
            return rslt;
        }

        constexpr T PerBase() const override { return angle.PerBase() / (velTime.PerBase() * accelTime.PerBase()); }
    };

    template <class T>
    using RadianPerSecondPerSecond = AngularAcceleration<T, Radian<T>, Second<T>, Second<T>>;

    template <class T, class OtherAngleUnit, class OtherVelTimeUnit, class OtherAccelTimeUnit, StringLiteral STR_1>
    RadianPerSecondPerSecond<T> operator/(
        const AngularVelocity<T, OtherAngleUnit, OtherVelTimeUnit>& lhs,
        const Time<T, STR_1, OtherAccelTimeUnit>&                   rhs)
    {
        RadianPerSecondPerSecond<T> rslt(lhs.BaseValue() / rhs.BaseValue());
        return rslt;
    }

} // namespace Ignosi::Libraries::Math
