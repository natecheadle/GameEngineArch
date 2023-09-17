#pragma once

#include "ComponentPool.h"
#include "Entity.h"
#include "EntityPointer.h"
#include "IComponent.h"
#include "IEntity.h"
#include "ISystem.h"
#include "ResourceManager.h"
#include "System.h"
#include "Tag.h"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <queue>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <vector>

namespace Ignosi::Modules::ECS
{
    template <ComponentObject... ComponentTypes>
    class World : public IWorld
    {
        std::tuple<ComponentPool<ComponentTypes>...> m_Pools;

        std::vector<Entity<ComponentTypes...>> m_Entities;

        std::vector<EntityID> m_LivingEntities;
        std::queue<EntityID>  m_FreedEntities;
        std::vector<EntityID> m_ToDelete;
        std::vector<EntityID> m_ToAdd;

        std::map<Tag, std::vector<EntityID>>  m_TaggedLists;
        std::vector<std::pair<Tag, EntityID>> m_TaggedEntitiesToAdd;
        std::vector<std::pair<Tag, EntityID>> m_TaggedEntitiesToRemove;

        std::vector<std::unique_ptr<ISystem>> m_Systems;

        ResourceManager m_ResourceManager;

        friend EntityPointer<ComponentTypes...>;

      public:
        World()          = default;
        virtual ~World() = default;

        World(const World& other) = delete;
        World(World&& other)      = default;

        World& operator=(const World& other) = delete;
        World& operator=(World&& other)      = default;

        IEntity* GetEntity(EntityID id) override
        {
            assert(id.ID < m_Entities.size());
            assert(std::find(m_LivingEntities.begin(), m_LivingEntities.end(), id) != m_LivingEntities.end());
            return &m_Entities[id.ID];
        }

        const IEntity* GetEntity(EntityID id) const override
        {
            assert(id.ID < m_Entities.size());
            assert(std::find(m_LivingEntities.begin(), m_LivingEntities.end(), id) != m_LivingEntities.end());
            const Entity<ComponentTypes...>& entity = m_Entities[id.ID];
            return static_cast<const IEntity*>(&entity);
        }

        ISystem* GetSystem(SystemID id) override
        {
            assert(id.ID < m_Systems.size());
            return m_Systems[id.ID].get();
        }

        template <class SystemType>
        SystemType* GetSystem()
        {
            for (auto& system : m_Systems)
            {
                SystemType* pSys = dynamic_cast<SystemType*>(system.get());
                if (pSys)
                {
                    return pSys;
                }
            }

            return nullptr;
        }

        template <class... Components>
        EntityPointer<ComponentTypes...> CreateEntity(Components... components)
        {
            Entity<ComponentTypes...>* pEntity{nullptr};
            if (!m_FreedEntities.empty())
            {
                EntityID newId = m_FreedEntities.front();
                m_FreedEntities.pop();
                pEntity = &m_Entities[newId.ID];
            }
            else
            {
                EntityID newID{m_Entities.size()};
                m_Entities.push_back(std::move(Entity<ComponentTypes...>(newID)));
                pEntity = &m_Entities[m_Entities.size() - 1];
            }

            m_ToAdd.push_back(pEntity->ID());

            pEntity->Revive();

            (AddComponent<Components>(pEntity, std::move(components)), ...);

            return {pEntity->ID(), this};
        }

        template <class... Components>
        EntityPointer<ComponentTypes...> CreateEntity()
        {
            Entity<ComponentTypes...>* pEntity{nullptr};
            if (!m_FreedEntities.empty())
            {
                EntityID newId = m_FreedEntities.front();
                m_FreedEntities.pop();
                pEntity = &m_Entities[newId.ID];
            }
            else
            {
                EntityID newID{m_Entities.size()};
                m_Entities.push_back(std::move(Entity<ComponentTypes...>(newID)));
                pEntity = &m_Entities[m_Entities.size() - 1];
            }

            m_ToAdd.push_back(pEntity->ID());

            pEntity->Revive();

            (AddComponent<Components>(pEntity), ...);

            return {pEntity->ID(), this};
        }

        void Update(double dt) override
        {
            ManageEntityLifespans();
            for (auto& pSystem : m_Systems)
            {
                pSystem->Update(dt);
            }
        }

        template <typename T, class... Components>
        T* CreateSystem()
        {
            static_assert(std::is_base_of_v<System<Components...>, T>, "T must be derive from System.");
            std::unique_ptr<T> pNewSystem = std::make_unique<T>(&(std::get<ComponentPool<Components>>(m_Pools))...);
            pNewSystem->Initialize(m_Systems.size(), this);
            T* pReturn = pNewSystem.get();
            m_Systems.push_back(std::move(pNewSystem));
            std::sort(m_Systems.begin(), m_Systems.end(), [](const std::unique_ptr<ISystem>& lhs, const std::unique_ptr<ISystem>& rhs) {
                return lhs->Priority() < rhs->Priority();
            });

            return pReturn;
        }

        bool AddTag(const Tag& tag, EntityID entity) override
        {
            if (m_Entities.at(entity.ID).AddTag(tag))
            {
                m_TaggedEntitiesToAdd.push_back({tag, entity});
                return true;
            }
            return false;
        }

        bool AddTag(const Tag& tag, EntityPointer<ComponentTypes...>& pEntity) { return AddTag(tag, pEntity->ID()); }

        bool RemoveTag(const Tag& tag, EntityID entity) override
        {
            if (m_Entities.at(entity.ID).RemoveTag(tag))
            {
                m_TaggedEntitiesToRemove.push_back({tag, entity});
                return true;
            }
            return false;
        }

        bool RemoveTag(const Tag& tag, EntityPointer<ComponentTypes...>& pEntity) { return RemoveTag(tag, pEntity.get()); }

        const std::vector<EntityID>& AllEntities() const override { return m_LivingEntities; }
        const std::vector<EntityID>& GetEntitiesByTag(const Tag& tag) const override
        {
            static const std::vector<EntityID> emptyVector;
            auto                               it = m_TaggedLists.find(tag);
            return it != m_TaggedLists.end() ? m_TaggedLists.at(tag) : emptyVector;
        }

        const ResourceManager& Resources() const override { return m_ResourceManager; }
        ResourceManager&       Resources() override { return m_ResourceManager; }

        template <class ComponentType>
        void AddComponent(const IEntity* pEntity)
        {
            AddComponent(pEntity, ComponentType());
        }

        template <class ComponentType>
        void AddComponent(const IEntity* pEntity, ComponentType value)
        {
            if (!std::get<ComponentPool<ComponentType>>(m_Pools).HasComponent(pEntity))
            {
                auto component = CreateComponent<ComponentType>(pEntity->ID(), std::move(value));
                AddTag(component->Tag(), pEntity->ID());
                m_Entities[pEntity->ID().ID].template InitializeComponent(std::move(component));
            }
        }

      private:
        void DestroyEntity(EntityID id)
        {
            assert(id.ID < m_Entities.size());
            if (m_Entities[id.ID].IsAlive())
            {
                m_Entities[id.ID].Kill();
                m_ToDelete.push_back(m_Entities[id.ID].ID());
            }
        }

        template <typename ComponentType>
        ComponentPointer<ComponentType> CreateComponent(EntityID id)
        {
            return CreateComponent(id, ComponentType());
        }

        template <typename ComponentType>
        ComponentPointer<ComponentType> CreateComponent(EntityID id, ComponentType init)
        {
            auto& pool = std::get<ComponentPool<ComponentType>>(m_Pools);

            return std::move(pool.CreateComponent(std::move(init), id));
        }

        void ManageEntityLifespans()
        {
            for (auto& taggedEntities : m_TaggedEntitiesToRemove)
            {
                auto& tag_list = m_TaggedLists.at(taggedEntities.first);
                auto  it       = std::find(tag_list.begin(), tag_list.end(), taggedEntities.second);
                assert(it != tag_list.end());
                tag_list.erase(it);
            }

            m_TaggedEntitiesToRemove.clear();

            for (auto destroyID : m_ToDelete)
            {
                auto it = std::lower_bound(m_LivingEntities.begin(), m_LivingEntities.end(), destroyID);

                assert(it != m_LivingEntities.end());
                m_LivingEntities.erase(it);
                m_FreedEntities.push(destroyID);

                for (auto& taggedLists : m_TaggedLists)
                {
                    auto it = std::lower_bound(taggedLists.second.begin(), taggedLists.second.end(), destroyID);
                    if (it != taggedLists.second.end())
                    {
                        taggedLists.second.erase(it);
                    }
                }
            }

            for (auto addID : m_ToAdd)
            {
                m_LivingEntities.push_back(addID);
            }

            if (!m_ToAdd.empty())
            {
                // Sorting by pointer address because it is the same result as ID
                std::sort(m_LivingEntities.begin(), m_LivingEntities.end());
            }

            for (auto& taggedEntities : m_TaggedEntitiesToAdd)
            {
                if (!m_TaggedLists.contains(taggedEntities.first))
                {
                    m_TaggedLists.insert({taggedEntities.first, {taggedEntities.second}});
                }
                else
                {

                    auto& tag_list = m_TaggedLists.at(taggedEntities.first);
#ifdef DEBUG
                    assert(std::find(tag_list.begin(), tag_list.end(), taggedEntities.second) == tag_list.end());
#endif
                    tag_list.push_back(taggedEntities.second);
                    std::sort(tag_list.begin(), tag_list.end());
                }
            }

            m_TaggedEntitiesToAdd.clear();
            m_ToAdd.clear();
            m_ToDelete.clear();
        }
    };
} // namespace Ignosi::Modules::ECS
