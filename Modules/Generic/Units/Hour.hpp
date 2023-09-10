#pragma once

#include "Time.hpp"

namespace Ignosi::Modules
{
    template <class T>
    class Hour : public Time<T, Hour<T>>
    {
        friend Time<T, Hour<T>>;

      public:
        Hour() = default;
        constexpr Hour(float val)
            : Time<T, Hour>(val)
        {
        }

        template <class DERIVED_OTHER>
        Hour(const Time<T, DERIVED_OTHER>& other)
            : Time<T, Hour<T>>(other.Seconds() * ValPerSec())
        {
        }

        virtual ~Hour() = default;

        float ValPerSec() const override { return Time<T, Hour<T>>::HourPerSec(); };
    };
} // namespace Ignosi::Modules
