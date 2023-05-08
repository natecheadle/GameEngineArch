#pragma once

#include "FloatComp.hpp"

#include <type_traits>

namespace nate::Modules
{
    template <class T, class DERIVED>
    class Angle
    {
        T m_RadValue{0.0};

      protected:
        Angle(T val)
            : m_RadValue(val)
        {
        }
        Angle() = default;

      public:
        virtual ~Angle() = default;

        template <class DERIVED_OTHER>
        friend bool operator==(const Angle& lhs, const Angle<T, DERIVED_OTHER>& rhs)
        {
            return almost_eq(lhs.Radians(), rhs.Radians());
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

        T Radians() const { return m_RadValue; }

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

      protected:
        Angle(Angle& other) = default;

        virtual T          ValPerRad() const = 0;
        static constexpr T DegreePerRad() { return 180.0f / T(M_PI); }
        static constexpr T RadPerDegree() { return T(M_PI) / 180.0f; }
    };

    template <class T, class DERIVED>
    T sin(const Angle<T, DERIVED>& val)
    {
        return std::sin(val.Radians());
    }

    template <class T, class DERIVED>
    T cos(const Angle<T, DERIVED>& val)
    {
        return std::cos(val.Radians());
    }

    template <class T, class DERIVED>
    T tan(const Angle<T, DERIVED>& val)
    {
        return std::tan(val.Radians());
    }
} // namespace nate::Modules