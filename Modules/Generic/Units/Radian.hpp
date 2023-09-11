#pragma once

#include "Angle.hpp"

namespace Ignosi::Modules
{
    template <class T>
    class Radian : public Angle<T, Radian<T>>
    {
        using BASE = Angle<T, Radian<T>>;

      public:
        constexpr Radian(T val)
            : BASE(val)
        {
        }

        constexpr Radian()              = default;
        Radian(const Radian& other)     = default;
        Radian(Radian&& other) noexcept = default;
        virtual ~Radian()               = default;

        Radian& operator=(const Radian& other) = default;
        Radian& operator=(Radian&& other)      = default;

        template <class OtherDerived>
        Radian(const Angle<T, OtherDerived>& other)
            : BASE(0)
        {
            BASE::BASE::BaseValue(other.BaseValue());
        }

        template <class OtherDerived>
        Radian& operator=(const Angle<T, OtherDerived>& other)
        {
            BASE::BASE::BaseValue(other.BaseValue());
            return *this;
        }

        constexpr T PerBase() const override { return static_cast<T>(1.0); }
    };
} // namespace Ignosi::Modules
