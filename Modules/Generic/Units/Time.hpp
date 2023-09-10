#pragma once

#include "FloatComp.hpp"

#include <type_traits>

namespace Ignosi::Modules
{
    template <class T, class DERIVED>
    class Time
    {
        T m_Seconds{0.0};

      protected:
        constexpr Time(T val)
            : m_Seconds(val)
        {
        }
        Time() = default;

      public:
        virtual ~Time() = default;

        template <class DERIVED_OTHER>
        friend bool operator==(const Time& lhs, const Time<T, DERIVED_OTHER>& rhs)
        {
            return almost_eq(lhs.Seconds(), rhs.Seconds());
        }

        template <class DERIVED_OTHER>
        friend bool operator<(const Time& lhs, const Time<T, DERIVED_OTHER>& rhs)
        {
            return lhs.m_Seconds < rhs.Seconds();
        }

        template <class DERIVED_OTHER>
        friend bool operator>(const Time& lhs, const Time<T, DERIVED_OTHER>& rhs)
        {
            return lhs.m_Seconds > rhs.Seconds();
        }

        template <class DERIVED_OTHER>
        friend bool operator<=(const Time& lhs, const Time<T, DERIVED_OTHER>& rhs)
        {
            return lhs.m_Seconds < rhs.Seconds() || lhs == rhs;
        }

        template <class DERIVED_OTHER>
        friend bool operator>=(const Time& lhs, const Time<T, DERIVED_OTHER>& rhs)
        {
            return lhs.m_Seconds > rhs.Seconds() || lhs == rhs;
        }

        template <class DERIVED_OTHER>
        Time& operator=(const Time<T, DERIVED_OTHER>& other)
        {
            if (this == &other)
                return *this;

            m_Seconds = other.m_Seconds;
            return *this;
        }

        void Value(T val) { m_Seconds = val / static_cast<DERIVED*>(this)->ValPerSec(); }
        T    Value() const { return static_cast<DERIVED*>(this)->ValPerSec() * m_Seconds; }

        constexpr T Seconds() const { return m_Seconds; }

        template <class DERIVED_OTHER>
        friend DERIVED operator-(const Time& lhs, const Time<T, DERIVED_OTHER>& rhs)
        {
            DERIVED rslt;
            rslt.m_Seconds = lhs.Seconds() - rhs.Seconds();
            return rslt;
        }

        template <class DERIVED_OTHER>
        friend DERIVED operator+(const Time& lhs, const Time<T, DERIVED_OTHER>& rhs)
        {
            DERIVED rslt;
            rslt.m_Seconds = lhs.Seconds() + rhs.Seconds();
            return rslt;
        }

        friend DERIVED operator*(const Time& lhs, T rhs)
        {
            DERIVED rslt;
            rslt.m_Seconds = lhs.Seconds() * rhs;
            return rslt;
        }

        friend DERIVED operator/(const Time& lhs, T rhs)
        {
            DERIVED rslt;
            rslt.m_Seconds = lhs.Seconds() / rhs;
            return rslt;
        }

        template <class DERIVED_OTHER>
        friend Time& operator-=(Time& lhs, const Time<T, DERIVED_OTHER>& rhs)
        {
            lhs.m_Seconds -= rhs.Seconds();
            return lhs;
        }

        template <class DERIVED_OTHER>
        friend Time& operator+=(Time& lhs, const Time<T, DERIVED_OTHER>& rhs)
        {
            lhs.m_Seconds += rhs.Seconds();
            return lhs;
        }

        friend Time& operator*=(Time& lhs, T rhs)
        {
            lhs.m_Seconds *= rhs;
            return lhs;
        }

        friend Time& operator/=(Time& lhs, T rhs)
        {
            lhs.m_Seconds /= rhs;
            return lhs;
        }

        virtual T ValPerSec() const = 0;

      protected:
        Time(Time& other) = default;

        static constexpr T MinPerSec() { return T(1.0f) / T(60.0f); }
        static constexpr T HourPerSec() { return T(1.0f) / T(60.0f * 60.0f); }
    };
} // namespace Ignosi::Modules