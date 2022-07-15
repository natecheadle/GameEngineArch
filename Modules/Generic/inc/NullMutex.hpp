#pragma once

#include <chrono>
namespace nate::Modules {
    class NullMutex {
      public:
        static void lock() {}
        static void unlock() {}
        static bool try_lock() { return true; }

        template <class Rep, class Period>
        static bool try_lock_for(const std::chrono::duration<Rep, Period>&)
        {
            return true;
        }

        template <class Clock, class Duration>
        static bool try_lock_until(const std::chrono::time_point<Clock, Duration>&)
        {
            return true;
        }
    };
}; // namespace nate::Modules