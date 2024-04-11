#pragma once

#include "Queue.hpp"

#include <mutex>

namespace Ignosi::Libraries::Containers
{
    template <typename T>
    class ConcurrentQueue final
    {
        mutable std::mutex m_QueueMutex;
        Queue<T>           m_Queue;

      public:
        ConcurrentQueue()  = default;
        ~ConcurrentQueue() = default;

        ConcurrentQueue(const ConcurrentQueue& other)
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            std::unique_lock<std::mutex> lockother(other.m_QueueMutex);
            m_Queue = other.m_Queue;
        }

        ConcurrentQueue(ConcurrentQueue&& other) noexcept
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            std::unique_lock<std::mutex> lockother(other.m_QueueMutex);
            m_Queue = std::move(other.m_Queue);
        }

        ConcurrentQueue& operator=(const ConcurrentQueue& other)
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            std::unique_lock<std::mutex> lockother(other.m_QueueMutex);
            m_Queue = other.m_Queue;

            return *this;
        }

        ConcurrentQueue& operator=(ConcurrentQueue&& other) noexcept
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            std::unique_lock<std::mutex> lockother(other.m_QueueMutex);
            m_Queue = std::move(other.m_Queue);

            return *this;
        }

        friend bool operator==(const ConcurrentQueue& lhs, const ConcurrentQueue& rhs)
        {
            std::unique_lock<std::mutex> locklhs(lhs.m_QueueMutex);
            std::unique_lock<std::mutex> lockrhs(rhs.m_QueueMutex);

            return lhs.m_Queue == rhs.m_Queue;
        }

        T& push(const T& newObj)
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            return m_Queue.push(newObj);
        }

        T& push(T&& newObj)
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            return m_Queue.push(std::forward<T>(newObj));
        }

        T pop()
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            return m_Queue.pop();
        }

        bool empty() const
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            return m_Queue.empty();
        }

        void clear()
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            m_Queue.clear();
        }
    };
} // namespace Ignosi::Libraries::Containers
