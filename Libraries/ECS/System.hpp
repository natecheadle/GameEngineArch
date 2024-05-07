#pragma once

#include "ObjectPool.hpp"
#include "PoolPointer.hpp"

#include <chrono>

namespace Ignosi::Libraries::ECS
{
    template <typename T>
    class System
    {
        Containers::ObjectPool<T> m_ComponentPool;

      public:
        virtual ~System() = default;

        virtual void Update(std::chrono::milliseconds delta) = 0;

        Containers::PoolPointer<T> CreateComponent() { return m_ComponentPool.Create(); }
        Containers::PoolPointer<T> CreateComponent(T&& newComponent) { return m_ComponentPool.Create(std::forward<T>(newComponent)); }
        Containers::PoolPointer<T> CreateComponent(const T& newComponent) { return m_ComponentPool.Create(newComponent); }

      protected:
        Containers::ObjectPool<T>&       ComponentPool() { return m_ComponentPool; }
        const Containers::ObjectPool<T>& ComponentPool() const { return m_ComponentPool; }
    };
} // namespace Ignosi::Libraries::ECS
