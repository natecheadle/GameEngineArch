#pragma once

#include "Component.hpp"
#include "ISystem.h"
#include "ECSPool.hpp"

namespace Ignosi::Libraries::ECS
{

    template <typename T>
    class System : public ISystem
    {
        template <typename... COMPONENTS>
        friend class World;

        ECSPool<Component<T>>* m_ComponentPool;
        size_t                 m_ID;

      public:
        virtual ~System() = default;

        size_t ID() const override { return m_ID; }

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
