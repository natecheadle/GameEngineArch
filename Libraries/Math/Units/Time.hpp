#pragma once

#include "Unit.hpp"

namespace Ignosi::Libraries::Math
{
    template <class T, StringLiteral Units, class Derived>
    class Time : public Unit<T, UnitType::Time, Units, Derived>
    {
      protected:
        using BASE = Unit<T, UnitType::Time, Units, Derived>;

        constexpr Time(T val)
            : BASE(val)
        {
        }

        constexpr Time()            = default;
        Time(const Time& other)     = default;
        Time(Time&& other) noexcept = default;
        virtual ~Time()             = default;

        Time& operator=(const Time& other) = default;
        Time& operator=(Time&& other)      = default;
    };

} // namespace Ignosi::Libraries::Math
