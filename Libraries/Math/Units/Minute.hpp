#pragma once

#include "Time.hpp"

namespace Ignosi::Libraries::Math
{
    template <class T>
    class Minute : public Time<T, Minute<T>>
    {
        using BASE = Time<T, Minute<T>>;

      public:
        constexpr Minute(T val)
            : BASE(val / PerBase())
        {
        }

        Minute()                        = default;
        Minute(const Minute& other)     = default;
        Minute(Minute&& other) noexcept = default;
        virtual ~Minute()               = default;

        Minute& operator=(const Minute& other) = default;
        Minute& operator=(Minute&& other)      = default;

        template <class OtherDerived>
        Minute(const Time<T, OtherDerived>& other)
            : BASE(0)
        {
            BASE::BASE::BaseValue(other.BaseValue());
        }

        template <class OtherDerived>
        Minute& operator=(const Time<T, OtherDerived>& other)
        {
            BASE::BASE::BaseValue(other.BaseValue());
            return *this;
        }

        constexpr T PerBase() const override { return static_cast<T>(1.0 / 60.0); }
    };
} // namespace Ignosi::Libraries::Math
