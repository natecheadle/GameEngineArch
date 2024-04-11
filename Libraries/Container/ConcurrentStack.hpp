#pragma once

#include "Stack.hpp"

#include <mutex>

namespace Ignosi::Libraries::Containers
{
    template <typename T>
    class ConcurrentStack final
    {
        mutable std::mutex m_StackLock;
        Stack<T>           m_Stack;

      public:
        ConcurrentStack()  = default;
        ~ConcurrentStack() = default;

        ConcurrentStack(const ConcurrentStack& other)
        {
            std::unique_lock<std::mutex> lock(m_StackLock);
            std::unique_lock<std::mutex> lock_other(other.m_StackLock);
            m_Stack = other.m_Stack;
        }

        ConcurrentStack(ConcurrentStack&& other) noexcept
        {
            std::unique_lock<std::mutex> lock(m_StackLock);
            std::unique_lock<std::mutex> lock_other(other.m_StackLock);
            m_Stack = std::move(other.m_Stack);
        }

        ConcurrentStack& operator=(const ConcurrentStack& other)
        {
            std::unique_lock<std::mutex> lock(m_StackLock);
            std::unique_lock             lock_other(other.m_StackLock);
            m_Stack = std::move(other.m_Stack);

            return *this;
        }

        ConcurrentStack& operator=(ConcurrentStack&& other) noexcept
        {
            std::unique_lock<std::mutex> lock(m_StackLock);
            std::unique_lock<std::mutex> lock_other(other.m_StackLock);
            m_Stack = std::move(other.m_Stack);

            return *this;
        }

        T& push(const T& newObj)
        {
            std::unique_lock<std::mutex> lock(m_StackLock);
            return m_Stack.push(newObj);
        }

        T& push(T&& newObj)
        {
            std::unique_lock<std::mutex> lock(m_StackLock);
            return m_Stack.push(std::forward<T>(newObj));
        }

        T pop()
        {
            std::unique_lock<std::mutex> lock(m_StackLock);
            return m_Stack.pop();
        }

        size_t size() const
        {
            std::unique_lock<std::mutex> lock(m_StackLock);
            return m_Stack.size();
        }

        bool empty() const
        {
            std::unique_lock<std::mutex> lock(m_StackLock);
            return m_Stack.empty();
        }

        void clear()
        {
            std::unique_lock<std::mutex> lock(m_StackLock);
            m_Stack.clear();
        }
    };

} // namespace Ignosi::Libraries::Containers
