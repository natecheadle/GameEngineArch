#pragma once

#include "Component.hpp"
#include "ObjectPool.hpp"
#include "PoolPointer.hpp"

#include <chrono>

namespace Ignosi::Libraries::ECS
{

    template <typename T>
    class System
    {
        template <typename... COMPONENTS>
        friend class World;

        Containers::ObjectPool<Component<T>> m_ComponentPool;

      public:
        virtual ~System() = default;

        virtual void Update(std::chrono::milliseconds delta) = 0;

      protected:
        Containers::ObjectPool<Component<T>>&       ComponentPool() { return m_ComponentPool; }
        const Containers::ObjectPool<Component<T>>& ComponentPool() const { return m_ComponentPool; }
    };
} // namespace Ignosi::Libraries::ECS
