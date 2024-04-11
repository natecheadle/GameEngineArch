#pragma once

#include <cassert>
#include <memory>
#include <optional>
#include <stdexcept>

namespace Ignosi::Libraries::Containers
{
    template <typename T>
    class Queue final
    {
        struct Node
        {
            Node(std::unique_ptr<Node> next)
                : Next(std::move(next))
                , Data(std::nullopt)
            {
            }

            Node(std::unique_ptr<Node> next, const T& data)
                : Next(std::move(next))
                , Data(data)
            {
            }

            Node(std::unique_ptr<Node> next, T&& data)
                : Next(std::move(next))
                , Data(std::forward<T>(data))
            {
            }

            std::unique_ptr<Node> Next;
            std::optional<T>      Data;
        };

        Node*                 m_End{nullptr};
        std::unique_ptr<Node> m_Begin{nullptr};

      public:
        Queue()  = default;
        ~Queue() = default;

        Queue(const Queue& other) { copyFromOther(other); }

        Queue(Queue&& other) noexcept
            : m_End(other.m_End)
            , m_Begin(std::move(other.m_Begin))
        {
            other.m_Begin = nullptr;
        }

        Queue& operator=(const Queue& other)
        {
            clear();
            copyFromOther(other);

            return *this;
        }

        Queue& operator=(Queue&& other) noexcept
        {
            m_End       = other.m_End;
            other.m_End = nullptr;
            m_Begin     = std::move(other.m_Begin);

            return *this;
        }

        friend bool operator==(const Queue& lhs, const Queue& rhs)
        {
            Node* pNext  = lhs.m_Begin.get();
            Node* pNext2 = rhs.m_Begin.get();
            while (pNext && pNext2)
            {
                if (pNext->Data != pNext2->Data)
                    return false;

                pNext  = pNext->Next.get();
                pNext2 = pNext2->Next.get();
            }

            return !pNext && !pNext2;
        }

        T& push(const T& newObj) { return push(T(newObj)); }

        T& push(T&& newObj)
        {

            if (m_End)
            {
                m_End->Next = std::make_unique<Node>(nullptr, std::forward<T>(newObj));
                m_End       = m_End->Next.get();
            }
            else
            {
                m_Begin = std::make_unique<Node>(nullptr, std::forward<T>(newObj));
                m_End   = m_Begin.get();
            }

            return m_End->Data.value();
        }

        T pop()
        {
            if (m_Begin)
            {
                std::unique_ptr<Node> begin = std::move(m_Begin);
                m_Begin                     = std::move(begin->Next);

                if (!m_Begin)
                    m_End = nullptr;

                return std::move(begin->Data.value());
            }
            throw std::out_of_range("container empty");
        }

        bool empty() const { return !m_Begin; }

        void clear()
        {
            m_Begin.reset();
            m_End = m_Begin.get();
        }

      private:
        void copyFromOther(const Queue& other)
        {
            Node* pNext = other.m_Begin.get();
            while (pNext)
            {
                push(pNext->Data.value());
                pNext = pNext->Next.get();
            }
        }
    };
} // namespace Ignosi::Libraries::Containers
