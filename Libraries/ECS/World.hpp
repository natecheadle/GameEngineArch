#pragma once

#include "Entity.hpp"
#include "PoolPointer.hpp"
#include "ResourceManager.h"
#include "System.hpp"

#include <ObjectPool.hpp>

#include <chrono>
#include <memory>
#include <tuple>

namespace Ignosi::Libraries::ECS
{
    template <typename... Components>
    class World
    {
        Containers::ObjectPool<Entity<Components...>>      m_EntityPool;
        std::tuple<std::unique_ptr<System<Components>>...> m_Systems;
        ResourceManager                                    m_Resources;

        friend Entity<Components...>;

      public:
        World() = default;

        ResourceManager&       Resources() { return m_Resources; }
        const ResourceManager& Resources() const { return m_Resources; }

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

        Containers::PoolPointer<Entity<Components...>> CreateEntity()
        {
            auto entity        = m_EntityPool.Create(Entity<Components...>(*this));
            entity.Get()->m_ID = entity.ID();
            return entity;
        }

        template <typename COMPONENT>
        System<COMPONENT>* GetSystem()
        {
            return std::get<std::unique_ptr<System<COMPONENT>>>(m_Systems).get();
        }

        std::optional<Entity<Components...>>&       GetEntity(size_t id) { return m_EntityPool[id]; }
        const std::optional<Entity<Components...>>& GetEntity(size_t id) const { return m_EntityPool[id]; }

      private:
        template <typename COMPONENT>
        Containers::PoolPointer<Component<COMPONENT>> CreateComponent(Entity<Components...>& entity)
        {
            return GetSystem<COMPONENT>()->m_ComponentPool.Create(Component<COMPONENT>(entity.ID(), COMPONENT()));
        }

        template <typename COMPONENT>
        Containers::PoolPointer<Component<COMPONENT>> CreateComponent(Entity<Components...>& entity, COMPONENT&& newComponent)
        {
            return GetSystem<COMPONENT>()->m_ComponentPool.Create(Component<COMPONENT>(entity.ID(), std::forward<COMPONENT>(newComponent)));
        }

        template <typename COMPONENT>
        Containers::PoolPointer<Component<COMPONENT>> CreateComponent(Entity<Components...>& entity, const COMPONENT& newComponent)
        {
            return GetSystem<COMPONENT>()->m_ComponentPool.Create(Component<COMPONENT>(entity.ID(), newComponent));
        }
    };
} // namespace Ignosi::Libraries::ECS
