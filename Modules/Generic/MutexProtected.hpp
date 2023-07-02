#pragma once

#include <functional>
#include <mutex>

namespace Ignosi::Modules
{
    template <class MUTEX, class LOCKED_ITEM>
    class MutexProtected
    {
        mutable MUTEX m_Mutex;
        LOCKED_ITEM   m_Data;

      public:
        MutexProtected() = default;
        MutexProtected(LOCKED_ITEM&& data)
            : m_Data(data)
        {
        }

        void execute(std::function<void(LOCKED_ITEM& val)> func)
        {
            std::unique_lock<MUTEX> lock(m_Mutex);
            func(m_Data);
        }

        void execute(std::function<void(const LOCKED_ITEM& val)> func) const
        {
            std::unique_lock<MUTEX> lock(m_Mutex);
            func(m_Data);
        }

        template <class T>
        T execute(std::function<T(LOCKED_ITEM& val)> func)
        {
            std::unique_lock<MUTEX> lock(m_Mutex);
            return func(m_Data);
        }

        template <class T>
        T execute(std::function<T(const LOCKED_ITEM& val)> func) const
        {
            std::unique_lock<MUTEX> lock(m_Mutex);
            return func(m_Data);
        }
    };
} // namespace Ignosi::Modules