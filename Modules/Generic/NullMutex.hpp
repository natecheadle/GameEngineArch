#pragma once

#include <chrono>

namespace Ignosi::Modules {
    class NullMutex {
      public:
        static void lock() noexcept {}
        static void unlock() noexcept {}
        static bool try_lock() noexcept { return true; }
        static void lock_shared() noexcept {}
        static void unlock_shared() noexcept {}

        template <class Rep, class Period>
        [[nodiscard]] static bool try_lock_for(const std::chrono::duration<Rep, Period>&) noexcept
        {
            return true;
        }

        template <class Clock, class Duration>
        [[nodiscard]] static bool try_lock_until(const std::chrono::time_point<Clock, Duration>&) noexcept
        {
            return true;
        }

        [[nodiscard]] static bool try_lock_shared() noexcept { return true; }
    };
}; // namespace Ignosi::Modules