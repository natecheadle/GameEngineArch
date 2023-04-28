#pragma once

#include <UID.hpp>

#include <utility>

namespace nate::Modules::ECS
{
    template <class T>
    class Component
    {
        const std::uint64_t m_ID;
        T             m_Data;

      public:
        Component(std::uint64_t id, T init = T())
            : m_ID(id)
            , m_Data(std::move(init))
        {
        }

        ~Component() = default;

        Component(const Component<T>& other) = delete;
        Component(Component<T>&& other) = delete;

        Component<T>& operator=(const Component<T>& other)
        {
            if (this == *other)
                return *this;

            m_Data = other.m_Data;
        }

        Component<T>& operator=(Component<T>&& other) = default;

        std::uint64_t ID() const { return m_ID; }
        const T&      Data() const { return m_Data; }
        T&            Data() { return m_Data; }

        friend bool operator==(const Component<T>& lhs, const Component<T>& rhs) { return lhs.m_Data == rhs.m_Data; }
    };
} // namespace nate::Modules::ECS
