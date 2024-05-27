#pragma once

#include "IComponent.h"

#include <cstddef>
#include <utility>

namespace Ignosi::Libraries::ECS
{
    template <typename T>
    class Component : public IComponent
    {
        size_t m_TypeID;
        size_t m_EntityID;
        T      m_Data;

      public:
        Component(size_t entityID, T data)
            : m_EntityID(entityID)
            , m_Data(std::move(data))
        {
        }

        ~Component() = default;

        Component(const Component& other) = delete;
        Component(Component&& other)      = default;

        Component& operator=(const Component& other) = delete;
        Component& operator=(Component&& other)      = default;

        size_t TypeID() const override { return m_TypeID; }
        size_t EntityID() const override { return m_EntityID; }

        const T& Data() const { return m_Data; }
        T&       Data() { return m_Data; }
    };
} // namespace Ignosi::Libraries::ECS
