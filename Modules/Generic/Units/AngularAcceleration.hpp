#pragma once

#include "AngularVelocity.hpp"
#include "FloatComp.hpp"
#include "Radian.hpp"
#include "Second.hpp"

#include <type_traits>

namespace Ignosi::Modules
{
    template <class T, class DerivedAngle, class DerivedTime1, class DerivedTime2>
    class AngularAcceleration
    {
        T m_RadPerSecPerSec;

        static_assert(std::is_base_of_v<Time<T, DerivedTime1>, DerivedTime1>, "DerivedTime1 must inherit from Time.");
        static_assert(std::is_base_of_v<Time<T, DerivedTime1>, DerivedTime2>, "DerivedTime2 must inherit from Time.");
        static_assert(std::is_base_of_v<Angle<T, DerivedAngle>, DerivedAngle>, "DerivedAngle must inherit from Time.");

      public:
        constexpr AngularAcceleration(T val)
            : m_RadPerSecPerSec(ToRadPerSecPerSec(val))
        {
        }
        AngularAcceleration() = default;

        AngularAcceleration(const AngularAcceleration& other) = default;

        template <class DerivedOtherTime1, class DerivedOtherTime2, class DerivedOtherAngle>
        AngularAcceleration(AngularAcceleration<T, DerivedOtherAngle, DerivedOtherTime1, DerivedOtherTime2>& other)
            : m_RadPerSecPerSec(other.RadPerSecPerSec())
        {
        }

        virtual ~AngularAcceleration() = default;

        template <class DerivedOtherTime1, class DerivedOtherTime2, class DerivedOtherAngle>
        friend bool operator==(
            const AngularAcceleration&                                                             lhs,
            const AngularAcceleration<T, DerivedOtherAngle, DerivedOtherTime1, DerivedOtherTime2>& rhs)
        {
            return almost_eq(lhs.RadPerSecPerSec(), rhs.RadPerSecPerSec());
        }

        template <class DerivedOtherTime1, class DerivedOtherTime2, class DerivedOtherAngle>
        friend bool operator<(
            const AngularAcceleration&                                                             lhs,
            const AngularAcceleration<T, DerivedOtherAngle, DerivedOtherTime1, DerivedOtherTime2>& rhs)
        {
            return lhs.m_RadPerSecPerSec < rhs.RadPerSecPerSec();
        }

        template <class DerivedOtherTime1, class DerivedOtherTime2, class DerivedOtherAngle>
        friend bool operator>(
            const AngularAcceleration&                                                             lhs,
            const AngularAcceleration<T, DerivedOtherAngle, DerivedOtherTime1, DerivedOtherTime2>& rhs)
        {
            return lhs.m_RadPerSecPerSec > rhs.RadPerSecPerSec();
        }

        template <class DerivedOtherTime1, class DerivedOtherTime2, class DerivedOtherAngle>
        friend bool operator<=(
            const AngularAcceleration&                                                             lhs,
            const AngularAcceleration<T, DerivedOtherAngle, DerivedOtherTime1, DerivedOtherTime2>& rhs)
        {
            return lhs.m_RadPerSecPerSec < rhs.RadPerSecPerSec() || lhs == rhs;
        }

        template <class DerivedOtherTime1, class DerivedOtherTime2, class DerivedOtherAngle>
        friend bool operator>=(
            const AngularAcceleration&                                                             lhs,
            const AngularAcceleration<T, DerivedOtherAngle, DerivedOtherTime1, DerivedOtherTime2>& rhs)
        {
            return lhs.m_RadPerSecPerSec > rhs.RadPerSecPerSec() || lhs == rhs;
        }

        template <class DerivedOtherTime1, class DerivedOtherTime2, class DerivedOtherAngle>
        AngularAcceleration& operator=(const AngularAcceleration<T, DerivedOtherAngle, DerivedOtherTime1, DerivedOtherTime2>& other)
        {
            if (this == &other)
                return *this;

            m_RadPerSecPerSec = other.m_RadPerSecPerSec;
            return *this;
        }

        void Value(T val) { m_RadPerSecPerSec = ToRadPerSecPerSec(val); }
        T    Value() const { return ToDerived(m_RadPerSecPerSec); }

        constexpr T RadPerSecPerSec() const { return m_RadPerSecPerSec; }

        template <class DerivedOtherTime1, class DerivedOtherTime2, class DerivedOtherAngle>
        friend AngularAcceleration operator-(
            const AngularAcceleration&                                                             lhs,
            const AngularAcceleration<T, DerivedOtherAngle, DerivedOtherTime1, DerivedOtherTime2>& rhs)
        {
            AngularAcceleration rslt;
            rslt.m_RadPerSecPerSec = lhs.RadPerSecPerSec() - rhs.RadPerSecPerSec();
            return rslt;
        }

        template <class DerivedOtherTime1, class DerivedOtherTime2, class DerivedOtherAngle>
        friend AngularAcceleration operator+(
            const AngularAcceleration&                                                             lhs,
            const AngularAcceleration<T, DerivedOtherAngle, DerivedOtherTime1, DerivedOtherTime2>& rhs)
        {
            AngularAcceleration rslt;
            rslt.m_RadPerSecPerSec = lhs.RadPerSecPerSec() + rhs.RadPerSecPerSec();
            return rslt;
        }

        friend AngularAcceleration operator*(const AngularAcceleration& lhs, T rhs)
        {
            AngularAcceleration rslt;
            rslt.m_RadPerSecPerSec = lhs.RadPerSecPerSec() * rhs;
            return rslt;
        }

        template <class OtherDerivedTime>
        friend AngularVelocity<T, DerivedAngle, DerivedTime1> operator*(
            const AngularAcceleration&       lhs,
            const Time<T, OtherDerivedTime>& rhs)
        {
            return AngularVelocity<T, DerivedAngle, DerivedTime1>(RadianPerSecond<T>(lhs.RadPerSecPerSec() * rhs.Seconds()));
        }

        friend AngularAcceleration operator/(const AngularAcceleration& lhs, T rhs)
        {
            AngularAcceleration rslt;
            rslt.m_RadPerSecPerSec = lhs.RadPerSecPerSec() / rhs;
            return rslt;
        }

        template <class DerivedOtherTime1, class DerivedOtherTime2, class DerivedOtherAngle>
        friend AngularAcceleration& operator-=(
            AngularAcceleration&                                                                   lhs,
            const AngularAcceleration<T, DerivedOtherAngle, DerivedOtherTime1, DerivedOtherTime2>& rhs)
        {
            lhs.m_RadPerSecPerSec -= rhs.RadPerSecPerSec();
            return lhs;
        }

        template <class DerivedOtherTime1, class DerivedOtherTime2, class DerivedOtherAngle>
        friend AngularAcceleration& operator+=(
            AngularAcceleration&                                                                   lhs,
            const AngularAcceleration<T, DerivedOtherAngle, DerivedOtherTime1, DerivedOtherTime2>& rhs)
        {
            lhs.m_RadPerSecPerSec += rhs.RadPerSecPerSec();
            return lhs;
        }

        friend AngularAcceleration& operator*=(AngularAcceleration& lhs, T rhs)
        {
            lhs.m_RadPerSecPerSec *= rhs;
            return lhs;
        }

        friend AngularAcceleration& operator/=(AngularAcceleration& lhs, T rhs)
        {
            lhs.m_RadPerSecPerSec /= rhs;
            return lhs;
        }

      private:
        static T ToRadPerSecPerSec(T value)
        {
            static const DerivedAngle angle;
            static const DerivedTime1 time1;
            static const DerivedTime2 time2;

            return value * time1.ValPerSec() * time2.ValPerSec() / angle.ValPerRad();
        }
        static T ToDerived(T value)
        {
            static const DerivedAngle angle;
            static const DerivedTime1 time1;
            static const DerivedTime2 time2;

            return value / (time1.ValPerSec() * time2.ValPerSec) * angle.ValPerRad();
        }
    };

    template <class T>
    using RadianPerSecondPerSecond = AngularAcceleration<T, Radian<T>, Second<T>, Second<T>>;
} // namespace Ignosi::Modules