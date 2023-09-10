#pragma once

#include "FloatComp.hpp"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <gcem.hpp>

#include <cmath>
#include <type_traits>

namespace Ignosi::Modules
{
    template <class T, class DERIVED>
    class Angle
    {
        T m_RadValue{0.0};

      protected:
        constexpr Angle(T val)
            : m_RadValue(val)
        {
        }
        Angle() = default;

        Angle(const Angle& other) = default;

      public:
        virtual ~Angle() = default;

        template <class DERIVED_OTHER>
        friend bool operator==(const Angle& lhs, const Angle<T, DERIVED_OTHER>& rhs)
        {
            return almost_eq(lhs.Radians(), rhs.Radians());
        }

        template <class DERIVED_OTHER>
        friend bool operator<(const Angle& lhs, const Angle<T, DERIVED_OTHER>& rhs)
        {
            return lhs.m_RadValue < rhs.Radians();
        }

        template <class DERIVED_OTHER>
        friend bool operator>(const Angle& lhs, const Angle<T, DERIVED_OTHER>& rhs)
        {
            return lhs.m_RadValue > rhs.Radians();
        }

        template <class DERIVED_OTHER>
        friend bool operator<=(const Angle& lhs, const Angle<T, DERIVED_OTHER>& rhs)
        {
            return lhs.m_RadValue < rhs.Radians() || lhs == rhs;
        }

        template <class DERIVED_OTHER>
        friend bool operator>=(const Angle& lhs, const Angle<T, DERIVED_OTHER>& rhs)
        {
            return lhs.m_RadValue > rhs.Radians() || lhs == rhs;
        }

        template <class DERIVED_OTHER>
        Angle& operator=(const Angle<T, DERIVED_OTHER>& other)
        {
            if (this == &other)
                return *this;

            m_RadValue = other.m_RadValue;
            return *this;
        }

        void Value(T val) { m_RadValue = val / static_cast<DERIVED*>(this)->ValPerRad(); }
        T    Value() const { return static_cast<DERIVED*>(this)->ValPerRad() * m_RadValue; }

        constexpr T Radians() const { return m_RadValue; }

        template <class DERIVED_OTHER>
        friend DERIVED operator-(const Angle& lhs, const Angle<T, DERIVED_OTHER>& rhs)
        {
            DERIVED rslt;
            rslt.m_RadValue = lhs.Radians() - rhs.Radians();
            return rslt;
        }

        template <class DERIVED_OTHER>
        friend DERIVED operator+(const Angle& lhs, const Angle<T, DERIVED_OTHER>& rhs)
        {
            DERIVED rslt;
            rslt.m_RadValue = lhs.Radians() + rhs.Radians();
            return rslt;
        }

        friend DERIVED operator*(const Angle& lhs, T rhs)
        {
            DERIVED rslt;
            rslt.m_RadValue = lhs.Radians() * rhs;
            return rslt;
        }

        friend DERIVED operator/(const Angle& lhs, T rhs)
        {
            DERIVED rslt;
            rslt.m_RadValue = lhs.Radians() / rhs;
            return rslt;
        }

        template <class DERIVED_OTHER>
        friend Angle& operator-=(Angle& lhs, const Angle<T, DERIVED_OTHER>& rhs)
        {
            lhs.m_RadValue -= rhs.Radians();
            return lhs;
        }

        template <class DERIVED_OTHER>
        friend Angle& operator+=(Angle& lhs, const Angle<T, DERIVED_OTHER>& rhs)
        {
            lhs.m_RadValue += rhs.Radians();
            return lhs;
        }

        friend Angle& operator*=(Angle& lhs, T rhs)
        {
            lhs.m_RadValue *= rhs;
            return lhs;
        }

        friend Angle& operator/=(Angle& lhs, T rhs)
        {
            lhs.m_RadValue /= rhs;
            return lhs;
        }

        virtual T ValPerRad() const = 0;

      protected:
        Angle(Angle& other) = default;

        static constexpr T DegreePerRad() { return 180.0f / T(M_PI); }
        static constexpr T RadPerDegree() { return T(M_PI) / 180.0f; }
    };

    template <class T, class DERIVED>
    constexpr T sin(const Angle<T, DERIVED>& val)
    {
        return gcem::sin(val.Radians());
    }

    template <class T, class DERIVED>
    constexpr T cos(const Angle<T, DERIVED>& val)
    {
        return gcem::cos(val.Radians());
    }

    template <class T, class DERIVED>
    constexpr T tan(const Angle<T, DERIVED>& val)
    {
        return gcem::tan(val.Radians());
    }
} // namespace Ignosi::Modules