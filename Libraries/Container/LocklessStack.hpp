#pragma once

#include <boost/atomic.hpp>

#include <cassert>
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace Ignosi::Libraries::Containers
{
    template <typename T>
    class LocklessStack final
    {
        struct Node
        {
            Node(Node* next)
                : Next(next)
                , Data(std::nullopt)
            {
            }

            Node(Node* next, const T& data)
                : Next(std::move(next))
                , Data(data)
            {
            }

            Node(Node* next, T&& data)
                : Next(std::move(next))
                , Data(std::forward<T>(data))
            {
            }

            Node*            Next;
            std::optional<T> Data;
        };

        struct Data
        {
            Node*  End;
            size_t Size;
        };

        boost::atomic<Data> m_Data{
            {
             .End  = nullptr,
             .Size = 0U,
             }
        };

      public:
        LocklessStack() = default;
        ~LocklessStack() { clear(); }

        LocklessStack(LocklessStack&& other) noexcept
        {
            m_Data.store(other.m_Data.exchange({
                nullptr,
                0,
            }));
        }

        LocklessStack& operator=(LocklessStack&& other) noexcept
        {
            m_Data.store(other.m_Data.exchange(nullptr));

            return *this;
        }
        bool is_lock_free() const { return m_Data.is_lock_free(); }

        void push(const T& newObj) { return push(T(newObj)); }

        void push(T&& newObj)
        {
            auto node = new Node(nullptr, std::forward<T>(newObj));

            Data oldValue = m_Data.load();
            do
            {
                node->Next = oldValue.End;
            } while (!m_Data.compare_exchange_weak(oldValue, {node, oldValue.Size + 1}));
        }

        T pop()
        {
            Data oldValue = m_Data.load();
            Data newValue = oldValue;
            do
            {
                if (oldValue.End == nullptr)
                {
                    throw std::out_of_range("container empty");
                }
                newValue.End  = oldValue.End->Next;
                newValue.Size = oldValue.Size - 1;

            } while (!m_Data.compare_exchange_weak(oldValue, newValue));

            T popVal = std::move(oldValue.End->Data.value());
            if (oldValue.End)
                delete oldValue.End;

            return popVal;
        }

        size_t size() const { return m_Data.load().Size; }
        bool   empty() const { return m_Data.load().End == nullptr; }

        void clear()
        {
            Data data = m_Data.exchange({nullptr, 0});

            Node* pEnd = data.End;
            while (pEnd != nullptr)
            {
                Node* dataNext = pEnd->Next;
                delete pEnd;

                pEnd = dataNext;
            }
        }
    };
} // namespace Ignosi::Libraries::Containers
