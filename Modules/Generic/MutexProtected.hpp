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

        MutexProtected(const MutexProtected& other)
        {
            std::unique_lock<MUTEX> lock(other.m_Mutex);
            m_Data = other.m_Data;
        }

        MutexProtected(MutexProtected&& other) noexcept
        {
            std::unique_lock<MUTEX> lock(other.m_Mutex);
            m_Data = std::move(other.m_Data);
        }

        MutexProtected& operator=(const MutexProtected& other)
        {
            if (*this == other)
                return *this;

            std::unique_lock<MUTEX> lock(other.m_Mutex);
            m_Data = other.m_Data;
            return *this;
        }

        MutexProtected& operator=(MutexProtected&& other) noexcept
        {
            std::unique_lock<MUTEX> lock(other.m_Mutex);
            m_Data = std::move(other.m_Data);
            return *this;
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