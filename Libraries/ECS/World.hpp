#pragma once

#include "Entity.hpp"
#include "PoolPointer.hpp"
#include "System.hpp"

#include <ObjectPool.hpp>

#include <chrono>
#include <concepts>
#include <memory>
#include <tuple>

namespace Ignosi::Libraries::ECS
{
    template <typename... Components>
    class World
    {
        Containers::ObjectPool<Entity<Components...>>      m_EntityPool;
        std::tuple<std::unique_ptr<System<Components>>...> m_Systems;

      public:
        World() = default;

        template <typename COMPONENT, std::derived_from<System<COMPONENT>> SYSTEM>
        SYSTEM* Register(std::unique_ptr<SYSTEM> system)
        {
            auto pSystem                                            = system.get();
            std::get<std::unique_ptr<System<COMPONENT>>>(m_Systems) = std::move(system);
            return pSystem;
        }

        void Update(std::chrono::milliseconds delta)

        {
            std::apply([delta](auto&&... args) { ((args->Update(delta)), ...); }, m_Systems);
        }

        Containers::PoolPointer<Entity<Components...>> CreateEntity() { return m_EntityPool.Create(Entity<Components...>(*this)); }

        template <typename COMPONENT>
        System<COMPONENT>* GetSystem()
        {
            return std::get<std::unique_ptr<System<COMPONENT>>>(m_Systems).get();
        }

        template <typename COMPONENT>
        Containers::PoolPointer<COMPONENT> CreateComponent()
        {
            return GetSystem<COMPONENT>()->CreateComponent();
        }

        template <typename COMPONENT>
        Containers::PoolPointer<COMPONENT> CreateComponent(COMPONENT&& newComponent)
        {
            return GetSystem<COMPONENT>()->CreateComponent(newComponent);
        }

        template <typename COMPONENT>
        Containers::PoolPointer<COMPONENT> CreateComponent(const COMPONENT& newComponent)
        {
            return GetSystem<COMPONENT>()->CreateComponent(newComponent);
        }
    };
} // namespace Ignosi::Libraries::ECS
