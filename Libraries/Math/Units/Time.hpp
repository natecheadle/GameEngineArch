#pragma once

#include "Unit.hpp"

namespace Ignosi::Libraries::Math
{
    template <class T, class Derived>
    class Time : public Unit<T, UnitType::Time, Derived>
    {
      protected:
        using BASE = Unit<T, UnitType::Time, Derived>;

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
