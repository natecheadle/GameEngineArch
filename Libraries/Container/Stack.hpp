#pragma once

#include <cassert>
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace Ignosi::Libraries::Containers
{
    template <typename T>
    class Stack final
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

        std::unique_ptr<Node> m_End{nullptr};
        size_t                m_Size{0};

      public:
        Stack()  = default;
        ~Stack() = default;

        Stack(const Stack& other) { copyFromOther(other); }

        Stack(Stack&& other) noexcept
            : m_End(std::move(other.m_End))
            , m_Size(other.m_Size)
        {
            other.m_Size = 0;
        }

        Stack& operator=(const Stack& other)
        {
            clear();

            copyFromOther(other);

            return *this;
        }

        Stack& operator=(Stack&& other) noexcept
        {
            m_End        = std::move(other.m_End);
            m_Size       = other.m_Size;
            other.m_Size = 0;

            return *this;
        }

        T& push(const T& newObj)
        {
            m_End = std::make_unique<Node>(std::move(m_End), newObj);
            m_Size++;
            return m_End->Data.value();
        }

        T& push(T&& newObj)
        {
            m_End = std::make_unique<Node>(std::move(m_End), std::forward<T>(newObj));
            m_Size++;
            return m_End->Data.value();
        }

        T pop()
        {
            if (m_End)
            {
                std::unique_ptr<Node> back = std::move(m_End);
                m_End                      = std::move(back->Next);
                m_Size--;

                return std::move(back->Data.value());
            }
            throw std::out_of_range("container empty");
        }

        size_t size() const { return m_Size; }
        bool   empty() const { return !m_End; }

        void clear() { m_End.reset(); }

      private:
        void copyFromOther(const Stack& other)
        {
            std::vector<const Node*> nodes;
            nodes.reserve(other.m_Size);
            const Node* pNode = other.m_End.get();

            while (pNode)
            {
                nodes.push_back(pNode);
                pNode = pNode->Next.get();
            }

            for (auto it = nodes.rbegin(); it != nodes.rend(); ++it)
            {
                push((*it)->Data.value());
            }
        }
    };
} // namespace Ignosi::Libraries::Containers
