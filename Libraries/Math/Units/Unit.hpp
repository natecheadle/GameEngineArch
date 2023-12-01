#pragma once

#include <FloatComp.hpp>
#include <StringLiteral.hpp>

#include <type_traits>

namespace Ignosi::Libraries::Math
{
    enum class UnitType
    {
        Time,
        Angle,
        Length,
        AngularVelocity,
        Velocity,
        AngularAcceleration,
        Acceleration,

        LAST
    };

    template <class T, UnitType TYPE, StringLiteral Units, class Derived>
    class Unit
    {
        T m_BaseValue{0.0};

      protected:
        constexpr Unit(T initValue)
            : m_BaseValue(initValue)
        {
        }

        constexpr Unit() = default;

        Unit(const Unit& other)     = default;
        Unit(Unit&& other) noexcept = default;

        Unit& operator=(const Unit& other)     = default;
        Unit& operator=(Unit&& other) noexcept = default;

        void BaseValue(T val) { m_BaseValue = val; }

      public:
        constexpr T    BaseValue() const { return m_BaseValue; }
        constexpr T    Value() const { return FromBase(m_BaseValue); }
        constexpr void Value(T value) { m_BaseValue = ToBase(value); }

        constexpr T         ToBase(T value) const { return value / PerBase(); }
        constexpr T         FromBase(T value) const { return value * PerBase(); }
        constexpr UnitType  Type() { return TYPE; }
        virtual constexpr T PerBase() const = 0;

        static constexpr std::string_view UnitString() { return Units.String(); }

        friend std::ostream& operator<<(std::ostream& os, const Unit& value)
        {
            os << value.Value() << " " << UnitString();
            return os;
        }

        template <class OtherDerived, StringLiteral OtherUnits>
        bool operator==(const Unit<T, TYPE, OtherUnits, OtherDerived>& other) const
        {
            return almost_eq(m_BaseValue, other.BaseValue());
        }

        template <class OtherDerived, StringLiteral OtherUnits>
        bool operator!=(const Unit<T, TYPE, OtherUnits, OtherDerived>& other) const
        {
            return !(*this == other);
        }

        template <class OtherDerived, StringLiteral OtherUnits>
        bool operator<(const Unit<T, TYPE, OtherUnits, OtherDerived>& other) const
        {
            return m_BaseValue < other.BaseValue();
        }

        template <class OtherDerived, StringLiteral OtherUnits>
        bool operator>(const Unit<T, TYPE, OtherUnits, OtherDerived>& other) const
        {
            return m_BaseValue > other.BaseValue();
        }

        template <class OtherDerived, StringLiteral OtherUnits>
        bool operator<=(const Unit<T, TYPE, OtherUnits, OtherDerived>& other) const
        {
            return (*this < other) || (*this == other);
        }

        template <class OtherDerived, StringLiteral OtherUnits>
        bool operator>=(const Unit<T, TYPE, OtherUnits, OtherDerived>& other) const
        {
            return (*this > other) || (*this == other);
        }

        template <class OtherDerived, StringLiteral OtherUnits>
        Derived operator+(const Unit<T, TYPE, OtherUnits, OtherDerived>& other) const
        {
            Derived rslt;
            rslt.m_BaseValue = m_BaseValue + other.BaseValue();
            return rslt;
        }

        template <class OtherDerived, StringLiteral OtherUnits>
        Derived& operator+=(const Unit<T, TYPE, OtherUnits, OtherDerived>& other)
        {
            m_BaseValue += other.BaseValue();
            return static_cast<Derived&>(*this);
        }

        template <class OtherDerived, StringLiteral OtherUnits>
        Derived operator-(const Unit<T, TYPE, OtherUnits, OtherDerived>& other) const
        {
            Derived rslt;
            rslt.m_BaseValue = m_BaseValue - other.BaseValue();
            return rslt;
        }

        template <class OtherDerived, StringLiteral OtherUnits>
        Derived& operator-=(const Unit<T, TYPE, OtherUnits, OtherDerived>& other)
        {
            m_BaseValue -= other.BaseValue();
            return static_cast<Derived&>(*this);
        }

        Derived operator*(T value) const
        {
            Derived rslt;
            rslt.m_BaseValue = m_BaseValue * value;
            return rslt;
        }

        Derived& operator*=(T other)
        {
            m_BaseValue *= other;
            return static_cast<Derived&>(*this);
        }

        Derived operator/(T value) const
        {
            Derived rslt;
            rslt.m_BaseValue = m_BaseValue / value;
            return rslt;
        }

        Derived& operator/=(T other)
        {
            m_BaseValue /= other;
            return static_cast<Derived&>(*this);
        }
    };

} // namespace Ignosi::Libraries::Math
