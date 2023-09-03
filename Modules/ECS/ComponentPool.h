#pragma once

#include "ComponentID.h"
#include "ComponentPointer.h"
#include "IEntity.h"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <queue>
#include <vector>

namespace Ignosi::Modules::ECS
{

    template <class T>
    class ComponentPool
    {
        std::queue<ComponentID>  m_FreedComponents;
        std::vector<T>           m_Components;
        std::vector<ComponentID> m_EntityLookup;

      public:
        bool HasComponent(const IEntity* pEntity) const
        {
            return m_EntityLookup.size() > pEntity->ID().ID && m_EntityLookup[pEntity->ID().ID].ID < m_Components.size();
        }

        T& GetComponent(const IEntity* pEntity)
        {
            assert(HasComponent(pEntity));
            return m_Components.at(m_EntityLookup.at(pEntity->ID().ID).ID);
        }

        const T& GetComponent(const IEntity* pEntity) const
        {
            assert(HasComponent(pEntity));
            return m_Components.atm_EntityLookup.at(pEntity->ID().ID);
        }

        T& GetComponent(ComponentID id)
        {
            assert(m_Components.size() > id.ID);
            return m_Components.at(id.ID);
        }

        const T& GetComponent(ComponentID id) const
        {
            assert(m_Components.size() > id.ID);
            return m_Components.at(id.ID);
        }

        ComponentPointer<T> CreateComponent(EntityID id) { return CreateComponent(T(), id); }

        ComponentPointer<T> CreateComponent(T&& component, EntityID id)
        {
            if (m_EntityLookup.size() <= id.ID)
            {
                m_EntityLookup.resize(id.ID + 1, ComponentID::Create());
            }

            if (!m_FreedComponents.empty())
            {
                ComponentPointer<T> newObj(m_FreedComponents.front(), this);
                m_FreedComponents.pop();
                m_Components.at(newObj.m_ID.ID) = std::move(component);
                m_EntityLookup[id.ID]           = newObj.m_ID;
                return newObj;
            }

            m_Components.push_back(std::move(component));
            ComponentID newId{.ID = m_Components.size() - 1};
            m_EntityLookup[id.ID] = newId;
            return ComponentPointer<T>(newId, this);
        }
    };
}; // namespace Ignosi::Modules::ECS
