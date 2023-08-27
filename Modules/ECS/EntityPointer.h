#pragma once

#include "Entity.h"

#include <limits>

namespace Ignosi::Modules::ECS
{
    template <typename... ComponentTypes>
    class World;

    template <typename... ComponentTypes>
    class EntityPointer
    {
        friend World<ComponentTypes...>;

        std::uint64_t             m_EntityID{std::numeric_limits<std::uint64_t>::max()};
        World<ComponentTypes...>* m_pWorld{nullptr};

        EntityPointer(std::uint64_t id, World<ComponentTypes...>* pWorld)
            : m_EntityID(id)
            , m_pWorld(pWorld)
        {
        }

      public:
        EntityPointer() = default;

        EntityPointer(const EntityPointer& other)            = delete;
        EntityPointer& operator=(const EntityPointer& other) = delete;

        EntityPointer(EntityPointer&& other)
        {
            m_EntityID       = other.m_EntityID;
            m_pWorld         = other.m_pWorld;
            other.m_EntityID = std::numeric_limits<std::uint64_t>::max();
            other.m_pWorld   = nullptr;
        }
        EntityPointer& operator=(EntityPointer&& other)
        {
            m_EntityID       = other.m_EntityID;
            m_pWorld         = other.m_pWorld;
            other.m_EntityID = std::numeric_limits<std::uint64_t>::max();
            other.m_pWorld   = nullptr;

            return *this;
        }

        ~EntityPointer() { reset(); }

        Entity<ComponentTypes...>* operator->()
        {
            assert(m_EntityID < m_pWorld->m_Entities.size());
            return &m_pWorld->m_Entities[m_EntityID];
        }
        const Entity<ComponentTypes...>* operator->() const
        {
            assert(m_EntityID < m_pWorld->m_Entities.size());
            return &m_pWorld->m_Entities[m_EntityID];
        }

        Entity<ComponentTypes...>& operator*()
        {
            assert(m_EntityID < m_pWorld->m_Entities.size());
            return m_pWorld->m_Entities[m_EntityID];
        }
        const Entity<ComponentTypes...>& operator*() const
        {
            assert(m_EntityID < m_pWorld->m_Entities.size());
            return m_pWorld->m_Entities[m_EntityID];
        }

        void reset()
        {
            if (m_pWorld && m_EntityID < m_pWorld->m_Entities.size())
                m_pWorld->DestroyEntity(m_EntityID);
            m_EntityID = std::numeric_limits<std::uint64_t>::max();
            m_pWorld   = nullptr;
        }

        Entity<ComponentTypes...>* get()
        {
            assert(m_EntityID < m_pWorld->m_Entities.size());
            return &m_pWorld->m_Entities[m_EntityID];
        }
        const Entity<ComponentTypes...>* get() const
        {
            assert(m_EntityID < m_pWorld->m_Entities.size());
            return &m_pWorld->m_Entities[m_EntityID];
        }
    };
} // namespace Ignosi::Modules::ECS