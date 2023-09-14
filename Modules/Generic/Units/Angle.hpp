#pragma once

#include "Unit.hpp"

#include <gcem.hpp>

#include <numbers>

namespace Ignosi::Modules
{
    template <class T, class Derived>
    class Angle : public Unit<T, UnitType::Angle, Derived>
    {
      protected:
        using BASE = Unit<T, UnitType::Angle, Derived>;

        constexpr Angle(T val)
            : BASE(val)
        {
        }

        constexpr Angle()             = default;
        Angle(const Angle& other)     = default;
        Angle(Angle&& other) noexcept = default;
        virtual ~Angle()              = default;

        Angle& operator=(const Angle& other) = default;
        Angle& operator=(Angle&& other)      = default;

      public:
        void Modulo() { BASE::BaseValue(std::fmod(BASE::BaseValue(), 2.0f * std::numbers::pi_v<float>)); }
    };

    template <class T, class Derived>
    constexpr T sin(const Angle<T, Derived>& val)
    {
        return gcem::sin(val.BaseValue());
    }

    template <class T, class Derived>
    constexpr T cos(const Angle<T, Derived>& val)
    {
        return gcem::cos(val.BaseValue());
    }

    template <class T, class Derived>
    constexpr T tan(const Angle<T, Derived>& val)
    {
        return gcem::tan(val.BaseValue());
    }

} // namespace Ignosi::Modules