#pragma once

#include <atomic>
#include <cassert>
#include <chrono>

namespace Ignosi::Modules {
    class DebugMutex {
#ifdef DEBUG
        std::atomic<bool> m_FakeMutex;
#endif
      public:
        void lock()
        {
#ifdef DEBUG
            assert(!m_FakeMutex);
            m_FakeMutex = true;
#endif
        }
        void unlock()
        {
#ifdef DEBUG
            m_FakeMutex = false;
#endif
        }
        bool try_lock()
        {
#ifdef DEBUG
            assert(!m_FakeMutex);
            m_FakeMutex = true;
            return true;
#endif
            return true;
        }

        template <class Rep, class Period>
        bool try_lock_for(const std::chrono::duration<Rep, Period>&)
        {
            return try_lock();
        }

        template <class Clock, class Duration>
        bool try_lock_until(const std::chrono::time_point<Clock, Duration>&)
        {
            return try_lock();
        }
    };
}; // namespace Ignosi::Modules