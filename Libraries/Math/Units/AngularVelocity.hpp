#pragma once

#include "Radian.hpp"
#include "Second.hpp"
#include "Unit.hpp"

namespace Ignosi::Libraries::Math
{
    template <class T, class AngleUnit, class TimeUnit>
    class AngularVelocity : public Unit<T, UnitType::AngularVelocity, AngularVelocity<T, AngleUnit, TimeUnit>>
    {
        using BASE = Unit<T, UnitType::AngularVelocity, AngularVelocity<T, AngleUnit, TimeUnit>>;
        static constexpr AngleUnit angle;
        static constexpr TimeUnit  time;

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

        template <class OtherAngleUnit, class OtherTimeUnit>
        AngularVelocity(const AngularVelocity<T, OtherAngleUnit, OtherTimeUnit>& other)
            : BASE(other.BaseValue())
        {
        }

        template <class OtherAngleUnit, class OtherTimeUnit>
        AngularVelocity& operator=(const AngularVelocity<T, OtherAngleUnit, OtherTimeUnit>& other)
        {
            if (this == &other)
                return *this;

            BASE::BaseValue(other.BaseValue());
            return *this;
        }

        template <class OtherTimeUnit>
        AngleUnit operator*(const Time<T, OtherTimeUnit>& other)
        {
            AngleUnit rslt;
            rslt.Value(rslt.FromBase(BASE::BaseValue() * other.BaseValue()));
            return rslt;
        }

        template <class OtherAngleUnit, class OtherTimeUnit>
        friend AngularVelocity operator/(const Angle<T, OtherAngleUnit>& lhs, const Time<T, OtherTimeUnit>& rhs)
        {
            AngularVelocity rslt;
            rslt.BaseValue(lhs.BaseValue() / rhs.BaseValue());
            return rslt;
        }

        constexpr T PerBase() const override { return angle.PerBase() / time.PerBase(); }
    };

    template <class T>
    using RadianPerSecond = AngularVelocity<T, Radian<T>, Second<T>>;
} // namespace Ignosi::Libraries::Math
