#pragma once

#include "Angle.hpp"

#include <numbers>

namespace Ignosi::Modules
{
    template <class T>
    class Degree : public Angle<T, Degree<T>>
    {
        using BASE = Angle<T, Degree<T>>;

      public:
        constexpr Degree(T val)
            : BASE(val / T(180.0) * std::numbers::pi_v<T>)
        {
        }

        constexpr Degree()              = default;
        Degree(const Degree& other)     = default;
        Degree(Degree&& other) noexcept = default;
        virtual ~Degree()               = default;

        Degree& operator=(const Degree& other) = default;
        Degree& operator=(Degree&& other)      = default;

        template <class OtherDerived>
        Degree(const Angle<T, OtherDerived>& other)
            : BASE(0)
        {
            BASE::BASE::BaseValue(other.BaseValue());
        }

        template <class OtherDerived>
        Degree& operator=(const Angle<T, OtherDerived>& other)
        {
            BASE::BASE::BaseValue(other.BaseValue());
            return *this;
        }

        constexpr T PerBase() const override { return static_cast<T>(180.0 / std::numbers::pi_v<double>); }
    };
} // namespace Ignosi::Modules
