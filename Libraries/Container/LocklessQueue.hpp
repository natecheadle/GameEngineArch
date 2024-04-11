#pragma once

#include <boost/atomic.hpp>

#include <cassert>
#include <memory>
#include <optional>
#include <stdexcept>

namespace Ignosi::Libraries::Containers
{
    template <typename T>
    class LocklessQueue final
    {
        struct Node
        {
            Node(Node* next)
                : Next(std::move(next))
                , Data(std::nullopt)
            {
            }

            Node(Node* next, const T& data)
                : Next(next)
                , Data(data)
            {
            }

            Node(Node* next, T&& data)
                : Next(next)
                , Data(std::forward<T>(data))
            {
            }

            Node*            Next;
            std::optional<T> Data;
        };

        struct Data
        {
            Node* End;
            Node* Begin;
        };

        boost::atomic<Data> m_Data{
            {
             .End   = nullptr,
             .Begin = nullptr,
             }
        };

        static constexpr Data EMPTY = Data{.End = nullptr, .Begin = nullptr};

      public:
        LocklessQueue()  = default;
        ~LocklessQueue() = default;

        LocklessQueue(const LocklessQueue& other) = delete;

        LocklessQueue(LocklessQueue&& other) noexcept
            : m_Data(other.m_Data.exchange(EMPTY))
        {
        }

        LocklessQueue& operator=(const LocklessQueue& other) = delete;

        LocklessQueue& operator=(LocklessQueue&& other) noexcept
        {
            m_Data.store(other.m_Data.exchange());

            return *this;
        }

        bool is_lock_free() const { return m_Data.is_lock_free(); }

        void push(const T& newObj) { return push(T(newObj)); }

        void push(T&& newObj)
        {
            Node* pNew     = new Node(nullptr, std::forward<T>(newObj));
            Data  oldValue = m_Data.load();
            Data  newValue = oldValue;

            do
            {
                if (oldValue.End)
                {
                    newValue.End->Next = pNew;
                    newValue.End       = pNew;
                }
                else
                {
                    newValue.Begin = pNew;
                    newValue.End   = pNew;
                }
            } while (!m_Data.compare_exchange_weak(oldValue, newValue));
        }

        T pop()
        {
            Data oldValue = m_Data.load();
            Data newValue = oldValue;
            do
            {
                if (oldValue.Begin)
                {
                    Node* begin    = oldValue.Begin;
                    newValue.Begin = begin->Next;

                    if (!newValue.Begin)
                        newValue.End = nullptr;
                }
                else
                {
                    throw std::out_of_range("container empty");
                }
            } while (!m_Data.compare_exchange_weak(oldValue, newValue));

            T popVal = std::move(oldValue.Begin->Data.value());
            if (oldValue.Begin)
                delete oldValue.Begin;

            return popVal;
        }

        bool empty() const { return !m_Data.load().Begin; }

        void clear()
        {
            Data oldData = m_Data.exchange({nullptr, nullptr});
            while (oldData.Begin)
            {
                Node* begin   = oldData.Begin;
                oldData.Begin = oldData.Begin->Next;

                delete begin;
            }
        }
    };
} // namespace Ignosi::Libraries::Containers
