#pragma once

#include "Time.hpp"

namespace Ignosi::Libraries::Math
{
    template <class T>
    class Second : public Time<T, "s", Second<T>>
    {
        using BASE = Time<T, "s", Second<T>>;

      public:
        constexpr Second(T val)
            : BASE(val)
        {
        }

        constexpr Second()              = default;
        Second(const Second& other)     = default;
        Second(Second&& other) noexcept = default;
        virtual ~Second()               = default;

        Second& operator=(const Second& other) = default;
        Second& operator=(Second&& other)      = default;

        template <class OtherDerived, StringLiteral OtherUnits>
        Second(const Time<T, OtherUnits, OtherDerived>& other)
            : BASE(0)
        {
            BASE::BASE::BaseValue(other.BaseValue());
        }

        template <class OtherDerived, StringLiteral OtherUnits>
        Second& operator=(const Time<T, OtherUnits, OtherDerived>& other)
        {
            BASE::BASE::BaseValue(other.BaseValue());
            return *this;
        }

        constexpr T PerBase() const override { return static_cast<T>(1.0); }
    };
} // namespace Ignosi::Libraries::Math
