#pragma once

#include "IEntity.h"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <queue>
#include <vector>

namespace Ignosi::Modules::ECS
{
    template <class T>
    class ComponentPointer;

    struct ComponentID
    {
        size_t ID;

        void                    Reset() { ID = RESET_VAL; }
        static constexpr size_t RESET_VAL = std::numeric_limits<size_t>::max();
    };

    template <class T>
    class ComponentPool
    {
        std::queue<ComponentID>  m_FreedComponents;
        std::vector<T>           m_Components;
        std::vector<ComponentID> m_EntityLookup;

      public:
        T&       GetComponent(const IEntity* pEntity) { return m_Components[m_EntityLookup[pEntity->ID()]]; }
        const T& GetComponent(const IEntity* pEntity) const { return m_Components[m_EntityLookup[pEntity->ID()]]; }
        T&       GetComponent(ComponentID id) { return m_Components[id.ID]; }
        const T& GetComponent(ComponentID id) const { return m_Components[id.ID]; }

        ComponentPointer<T> CreateComponent(EntityID id) { return CreateComponent(T(), id); }

        ComponentPointer<T> CreateComponent(T&& component, EntityID id)
        {
            if (m_EntityLookup.size() < id.ID)
            {
                m_EntityLookup.resize(id.ID);
            }

            if (!m_FreedComponents.empty())
            {
                ComponentPointer<T> newObj(m_FreedComponents.front(), this);
                m_FreedComponents.pop();
                m_Components[newObj.m_ID] = std::move(component);
                m_EntityLookup[id]        = newObj.m_ID;
                return std::move(newObj);
            }

            m_Components.push_back(std::move(component));
            ComponentID newId     = m_Components.size() - 1;
            m_EntityLookup[id.ID] = newId;
            return pointer(newId, this);
        }
    };
}; // namespace Ignosi::Modules::ECS
