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

        EntityID                  m_EntityID;
        World<ComponentTypes...>* m_pWorld{nullptr};

        EntityPointer(EntityID id, World<ComponentTypes...>* pWorld)
            : m_EntityID(id)
            , m_pWorld(pWorld)
        {
        }

      public:
        EntityPointer() = default;

        EntityPointer(const EntityPointer& other)            = delete;
        EntityPointer& operator=(const EntityPointer& other) = delete;

        EntityPointer(EntityPointer&& other) noexcept
        {
            m_EntityID     = other.m_EntityID;
            m_pWorld       = other.m_pWorld;
            other.m_pWorld = nullptr;
            other.m_EntityID.Reset();
        }
        EntityPointer& operator=(EntityPointer&& other) noexcept
        {
            m_EntityID     = other.m_EntityID;
            m_pWorld       = other.m_pWorld;
            other.m_pWorld = nullptr;
            other.m_EntityID.Reset();

            return *this;
        }

        ~EntityPointer() { reset(); }

        Entity<ComponentTypes...>* operator->()
        {
            assert(m_EntityID.ID < m_pWorld->m_Entities.size());
            return &m_pWorld->m_Entities[m_EntityID.ID];
        }
        const Entity<ComponentTypes...>* operator->() const
        {
            assert(m_EntityID.ID < m_pWorld->m_Entities.size());
            return &m_pWorld->m_Entities[m_EntityID.ID];
        }

        Entity<ComponentTypes...>& operator*()
        {
            assert(m_EntityID.ID < m_pWorld->m_Entities.size());
            return m_pWorld->m_Entities[m_EntityID.ID];
        }
        const Entity<ComponentTypes...>& operator*() const
        {
            assert(m_EntityID.ID < m_pWorld->m_Entities.size());
            return m_pWorld->m_Entities[m_EntityID.ID];
        }

        void reset()
        {
            if (m_pWorld && m_EntityID.ID < m_pWorld->m_Entities.size())
                m_pWorld->DestroyEntity(m_EntityID);
            m_EntityID.Reset();
            m_pWorld = nullptr;
        }

        bool IsValid() const { return m_pWorld != nullptr && m_EntityID.IsValid(); }

        Entity<ComponentTypes...>* get()
        {
            assert(m_EntityID.ID < m_pWorld->m_Entities.size());
            return &m_pWorld->m_Entities[m_EntityID.ID];
        }
        const Entity<ComponentTypes...>* get() const
        {
            assert(m_EntityID.ID < m_pWorld->m_Entities.size());
            return &m_pWorld->m_Entities[m_EntityID.ID];
        }
    };
} // namespace Ignosi::Modules::ECS