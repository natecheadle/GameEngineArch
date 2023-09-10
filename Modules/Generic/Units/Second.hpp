#pragma once

#include "Time.hpp"

namespace Ignosi::Modules
{
    template <class T>
    class Second : public Time<T, Second<T>>
    {
        friend Time<T, Second<T>>;

      public:
        Second() = default;
        constexpr Second(float val)
            : Time<T, Second>(val)
        {
        }

        template <class DERIVED_OTHER>
        Second(const Time<T, DERIVED_OTHER>& other)
            : Time<T, Second<T>>(other.Seconds())
        {
        }

        virtual ~Second() = default;

        float ValPerSec() const override { return 1.0; };
    };
} // namespace Ignosi::Modules
