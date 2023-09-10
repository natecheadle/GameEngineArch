#pragma once

#include "FloatComp.hpp"
#include "Radian.hpp"
#include "Second.hpp"
#include "Units/Radian.hpp"

#include <type_traits>

namespace Ignosi::Modules
{
    template <class T, class DerivedAngle, class DerivedTime>
    class AngularVelocity
    {
        T m_RadPerSec;

        static_assert(std::is_base_of_v<Time<T, DerivedTime>, DerivedTime>, "DerivedTime must inherit from Time.");
        static_assert(std::is_base_of_v<Angle<T, DerivedAngle>, DerivedAngle>, "DerivedAngle must inherit from Time.");

      public:
        constexpr AngularVelocity(T val)
            : m_RadPerSec(ToRadPerSec(val))
        {
        }
        AngularVelocity() = default;

        AngularVelocity(const AngularVelocity& other) = default;

        template <class DerivedOtherTime, class DerivedOtherAngle>
        AngularVelocity(AngularVelocity<T, DerivedOtherAngle, DerivedOtherTime>& other)
            : m_RadPerSec(other.RadPerSec())
        {
        }

        virtual ~AngularVelocity() = default;

        template <class DerivedOtherTime, class DerivedOtherAngle>
        friend bool operator==(const AngularVelocity& lhs, const AngularVelocity<T, DerivedOtherAngle, DerivedOtherTime>& rhs)
        {
            return almost_eq(lhs.RadPerSec(), rhs.RadPerSec());
        }

        template <class DerivedOtherTime, class DerivedOtherAngle>
        friend bool operator<(const AngularVelocity& lhs, const AngularVelocity<T, DerivedOtherAngle, DerivedOtherTime>& rhs)
        {
            return lhs.m_RadPerSec < rhs.RadPerSec();
        }

        template <class DerivedOtherTime, class DerivedOtherAngle>
        friend bool operator>(const AngularVelocity& lhs, const AngularVelocity<T, DerivedOtherAngle, DerivedOtherTime>& rhs)
        {
            return lhs.m_RadPerSec > rhs.RadPerSec();
        }

        template <class DerivedOtherTime, class DerivedOtherAngle>
        friend bool operator<=(const AngularVelocity& lhs, const AngularVelocity<T, DerivedOtherAngle, DerivedOtherTime>& rhs)
        {
            return lhs.m_RadPerSec < rhs.RadPerSec() || lhs == rhs;
        }

        template <class DerivedOtherTime, class DerivedOtherAngle>
        friend bool operator>=(const AngularVelocity& lhs, const AngularVelocity<T, DerivedOtherAngle, DerivedOtherTime>& rhs)
        {
            return lhs.m_RadPerSec > rhs.RadPerSec() || lhs == rhs;
        }

        template <class DerivedOtherTime, class DerivedOtherAngle>
        AngularVelocity& operator=(const AngularVelocity<T, DerivedOtherAngle, DerivedOtherTime>& other)
        {
            if (this == &other)
                return *this;

            m_RadPerSec = other.m_RadPerSec;
            return *this;
        }

        void Value(T val) { m_RadPerSec = ToRadPerSec(val); }
        T    Value() const { return ToDerived(m_RadPerSec); }

        constexpr T RadPerSec() const { return m_RadPerSec; }

        template <class DerivedOtherTime, class DerivedOtherAngle>
        friend AngularVelocity operator-(const AngularVelocity& lhs, const AngularVelocity<T, DerivedOtherAngle, DerivedOtherTime>& rhs)
        {
            AngularVelocity rslt;
            rslt.m_RadPerSec = lhs.RadPerSec() - rhs.RadPerSec();
            return rslt;
        }

        template <class DerivedOtherTime, class DerivedOtherAngle>
        friend AngularVelocity operator+(const AngularVelocity& lhs, const AngularVelocity<T, DerivedOtherAngle, DerivedOtherTime>& rhs)
        {
            AngularVelocity rslt;
            rslt.m_RadPerSec = lhs.RadPerSec() + rhs.RadPerSec();
            return rslt;
        }

        friend AngularVelocity operator*(const AngularVelocity& lhs, T rhs)
        {
            AngularVelocity rslt;
            rslt.m_RadPerSec = lhs.RadPerSec() * rhs;
            return rslt;
        }

        template <class OtherDerivedTime>
        friend DerivedAngle operator*(const AngularVelocity& lhs, const Time<T, OtherDerivedTime>& rhs)
        {
            return DerivedAngle(Radian<T>(lhs.RadPerSec() * rhs.Seconds()));
        }

        friend AngularVelocity operator/(const AngularVelocity& lhs, T rhs)
        {
            AngularVelocity rslt;
            rslt.m_RadPerSec = lhs.RadPerSec() / rhs;
            return rslt;
        }

        template <class DerivedOtherTime, class DerivedOtherAngle>
        friend AngularVelocity& operator-=(AngularVelocity& lhs, const AngularVelocity<T, DerivedOtherAngle, DerivedOtherTime>& rhs)
        {
            lhs.m_RadPerSec -= rhs.RadPerSec();
            return lhs;
        }

        template <class DerivedOtherTime, class DerivedOtherAngle>
        friend AngularVelocity& operator+=(AngularVelocity& lhs, const AngularVelocity<T, DerivedOtherAngle, DerivedOtherTime>& rhs)
        {
            lhs.m_RadPerSec += rhs.RadPerSec();
            return lhs;
        }

        friend AngularVelocity& operator*=(AngularVelocity& lhs, T rhs)
        {
            lhs.m_RadPerSec *= rhs;
            return lhs;
        }

        friend AngularVelocity& operator/=(AngularVelocity& lhs, T rhs)
        {
            lhs.m_RadPerSec /= rhs;
            return lhs;
        }

      private:
        static T ToRadPerSec(T value)
        {
            static DerivedAngle angle;
            static DerivedTime  time;

            return value * time.ValPerSec() / angle.ValPerRad();
        }
        static T ToDerived(T value)
        {
            static DerivedAngle angle;
            static DerivedTime  time;

            return value / time.ValPerSec() * angle.ValPerRad();
        }
    };

    template <class T>
    using RadianPerSecond = AngularVelocity<T, Radian<T>, Second<T>>;
} // namespace Ignosi::Modules