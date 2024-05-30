#pragma once

#include "Component.hpp"
#include "ECSPool.hpp"
#include "IComponent.h"
#include "IEntity.h"
#include "ISystem.h"
#include "IWorld.h"

namespace Ignosi::Libraries::ECS
{

    template <typename T>
    class System : public ISystem
    {
        template <typename... COMPONENTS>
        friend class World;

        const IWorld*          m_pParent;
        ECSPool<Component<T>>* m_ComponentPool;
        size_t                 m_ID;

      public:
        virtual ~System() = default;

        const IWorld*       Parent() const override { return m_pParent; }
        size_t              ID() const override { return m_ID; }
        const Component<T>* GetEntityComponent(size_t entityID)
        {
            const IEntity* pEntity = m_pParent->GetEntity(entityID);
            if (pEntity)
            {
                const IComponent* pComponent = pEntity->Get(m_ID);
                if (pComponent)
                {
                    return dynamic_cast<const Component<T>*>(pComponent);
                }
            }
            return nullptr;
        }

      protected:
        ECSPool<Component<T>>& ComponentPool()
        {
            assert(m_ComponentPool);
            return *m_ComponentPool;
        }

        const ECSPool<Component<T>>& ComponentPool() const
        {
            assert(m_ComponentPool);
            return *m_ComponentPool;
        }
    };
} // namespace Ignosi::Libraries::ECS
