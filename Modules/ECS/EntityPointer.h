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

        EntityID                  m_EntityID{EntityID::RESET_VAL};
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

        EntityPointer(EntityPointer&& other)
        {
            m_EntityID     = other.m_EntityID;
            m_pWorld       = other.m_pWorld;
            other.m_pWorld = nullptr;
            other.m_EntityID.Reset();
        }
        EntityPointer& operator=(EntityPointer&& other)
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