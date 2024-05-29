#pragma once

#include "ECSObject.hpp"
#include "ECSPool.hpp"
#include "Entity.hpp"
#include "IWorld.h"
#include "ResourceManager.h"
#include "System.hpp"

#include <chrono>
#include <memory>
#include <tuple>

namespace Ignosi::Libraries::ECS
{
    template <typename... COMPONENTS>
    class World : public IWorld
    {
        ECSPool<Entity<COMPONENTS...>>                     m_EntityPool;
        std::tuple<ECSPool<Component<COMPONENTS>>...>      m_ComponentPools;
        std::tuple<std::unique_ptr<System<COMPONENTS>>...> m_Systems;
        ResourceManager                                    m_Resources;

        friend Entity<COMPONENTS...>;

      public:
        World() = default;
        ~World()
        {
            m_EntityPool.Flush();
            std::apply([](auto&&... args) { ((args.Flush()), ...); }, m_ComponentPools);
        }

        ResourceManager&       Resources() { return m_Resources; }
        const ResourceManager& Resources() const { return m_Resources; }

        template <typename COMPONENT, std::derived_from<System<COMPONENT>> SYSTEM>
        SYSTEM* Register(std::unique_ptr<SYSTEM> system)
        {
            auto pSystem                                            = system.get();
            pSystem->m_ComponentPool                                = &(std::get<ECSPool<Component<COMPONENT>>>(m_ComponentPools));
            std::get<std::unique_ptr<System<COMPONENT>>>(m_Systems) = std::move(system);
            return pSystem;
        }

        void Update(std::chrono::milliseconds delta) override
        {
            std::apply([delta](auto&&... args) { ((args->Update(delta)), ...); }, m_Systems);
            m_EntityPool.Flush();

            std::apply([](auto&&... args) { ((args.Flush()), ...); }, m_ComponentPools);
        }

        IEntity* GetEntity(size_t id) override
        {
            std::optional<Entity<COMPONENTS...>>& rslt = GetConcreteEntity(id);
            return rslt.has_value() ? &rslt.value() : nullptr;
        }

        const IEntity* GetEntity(size_t id) const override
        {
            const std::optional<Entity<COMPONENTS...>>& rslt = GetConcreteEntity(id);
            return rslt.has_value() ? &rslt.value() : nullptr;
        }

        IComponent*       GetComponent(size_t entityID, size_t typeID) override { return GetEntity(entityID)->Get(typeID); }
        const IComponent* GetComponent(size_t entityID, size_t typeID) const override { return GetEntity(entityID)->Get(typeID); }

        ECSObject<Entity<COMPONENTS...>> CreateEntity()
        {
            auto entity        = m_EntityPool.Create(Entity<COMPONENTS...>(*this));
            entity.Get()->m_ID = entity.ID();
            return entity;
        }

        template <typename COMPONENT>
        System<COMPONENT>* GetSystem()
        {
            return std::get<std::unique_ptr<System<COMPONENT>>>(m_Systems).get();
        }

        std::optional<Entity<COMPONENTS...>>&       GetConcreteEntity(size_t id) { return m_EntityPool[id]; }
        const std::optional<Entity<COMPONENTS...>>& GetConcreteEntity(size_t id) const { return m_EntityPool[id]; }

      private:
        template <typename COMPONENT>
        ECSObject<Component<COMPONENT>> CreateComponent(Entity<COMPONENTS...>& entity)
        {
            return std::get<ECSPool<Component<COMPONENT>>>(m_ComponentPools)
                .Create(Component<COMPONENT>(entity.GetComponentTypeID<COMPONENT>(), entity.ID(), COMPONENT()));
        }

        template <typename COMPONENT>
        ECSObject<Component<COMPONENT>> CreateComponent(Entity<COMPONENTS...>& entity, COMPONENT&& newComponent)
        {
            return std::get<ECSPool<Component<COMPONENT>>>(m_ComponentPools)
                .Create(Component<COMPONENT>(entity.GetComponentTypeID<COMPONENT>(), entity.ID(), std::forward<COMPONENT>(newComponent)));
        }

        template <typename COMPONENT>
        ECSObject<Component<COMPONENT>> CreateComponent(Entity<COMPONENTS...>& entity, const COMPONENT& newComponent)
        {
            return std::get<ECSPool<Component<COMPONENT>>>(m_ComponentPools).Create(Component<COMPONENT>(entity.GetComponentTypeID<COMPONENT>(), entity.ID(), newComponent));
        }
    };
} // namespace Ignosi::Libraries::ECS
