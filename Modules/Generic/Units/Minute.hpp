#pragma once

#include "Time.hpp"

namespace Ignosi::Modules
{
    template <class T>
    class Minute : public Time<T, Minute<T>>
    {
        friend Time<T, Minute<T>>;

      public:
        Minute() = default;
        constexpr Minute(float val)
            : Time<T, Minute>(val)
        {
        }

        template <class DERIVED_OTHER>
        Minute(const Time<T, DERIVED_OTHER>& other)
            : Time<T, Minute<T>>(other.Seconds() * ValPerSec())
        {
        }

        virtual ~Minute() = default;

        float ValPerSec() const override { return Time<T, Minute<T>>::MinPerSec(); };
    };
} // namespace Ignosi::Modules
