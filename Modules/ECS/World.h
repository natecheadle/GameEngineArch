#pragma once

#include "Entity.h"
#include "EntityPointer.h"
#include "IEntity.h"
#include "System.h"
#include "Tag.h"

#include <PoolMemoryBlock.hpp>

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <memory>
#include <queue>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <vector>

namespace Ignosi::Modules::ECS
{

    template <typename... Types>
    class World
    {
        std::tuple<Memory::PoolMemoryBlock<Types>...> m_Pools;

        std::vector<Entity<Types...>> m_Entities;

        std::vector<EntityID> m_LivingEntities;
        std::queue<EntityID>  m_FreedEntities;
        std::vector<EntityID> m_ToDelete;
        std::vector<EntityID> m_ToAdd;

        std::map<Tag, std::vector<EntityID>>  m_TaggedLists;
        std::vector<std::pair<Tag, EntityID>> m_TaggedEntitiesToAdd;
        std::vector<std::pair<Tag, EntityID>> m_TaggedEntitiesToRemove;

        std::vector<std::unique_ptr<ISystem>> m_Systems;

        friend EntityPointer<Types...>;

      public:
        World()          = default;
        virtual ~World() = default;

        World(const World& other) = delete;
        World(World&& other)      = default;

        World& operator=(const World& other) = delete;
        World& operator=(World&& other)      = default;

        IEntity* GetEntity(EntityID id)
        {
            assert(id < m_Entities.size());
            assert(std::find(m_LivingEntities.begin(), m_LivingEntities.end(), id) != m_LivingEntities.end());
            return &m_Entities[id];
        }

        const IEntity* GetEntity(EntityID id) const
        {
            assert(id < m_Entities.size());
            assert(std::find(m_LivingEntities.begin(), m_LivingEntities.end(), id) != m_LivingEntities.end());
            return &m_Entities[id];
        }

        template <class... Components>
        EntityPointer<Types...> CreateEntity()
        {
            Entity<Types...>* pEntity{nullptr};
            if (!m_FreedEntities.empty())
            {
                EntityID newId = m_FreedEntities.front();
                m_FreedEntities.pop();
                pEntity = &m_Entities[newId];
            }
            else
            {
                EntityID newID = m_Entities.size();
                m_Entities.push_back(std::move(Entity<Types...>(newID)));
                pEntity = &m_Entities[m_Entities.size() - 1];
            }

            m_ToAdd.push_back(pEntity->ID());

            pEntity->Revive();
            (pEntity->template InitializeComponent<Components>(std::move(CreateComponent<Components>())), ...);

            return {pEntity->ID(), this};
        }

        void Update(double dt)
        {
            ManageEntityLifespans();
            for (auto& pSystem : m_Systems)
            {
                pSystem->Update(dt);
            }
        }

        template <typename T>
        T* CreateSystem()
        {
            static_assert(std::is_base_of_v<ISystem, T>, "T must be derive from System.");
            std::unique_ptr<ISystem> pNewSystem = std::make_unique<T>();
            pNewSystem->Initialize(static_cast<std::uint32_t>(m_Systems.size()));
            T* pReturn = pNewSystem.get();
            m_Systems.push_back(std::move(pNewSystem));
            std::sort(
                m_Systems.begin(),
                m_Systems.end(),
                [](const std::unique_ptr<ISystem>& lhs, const std::unique_ptr<ISystem>& rhs) {
                    return lhs->Priority() < rhs->Priority();
                });

            return pReturn;
        }

        bool AddTag(const Tag& tag, EntityID entity)
        {
            if (GetEntity(entity).AddTag(tag))
            {
                m_TaggedEntitiesToAdd.push_back({tag, entity});
                return true;
            }
            return false;
        }

        bool AddTag(const Tag& tag, EntityPointer<Types...>& pEntity) { return AddTag(tag, pEntity->ID()); }

        bool RemoveTag(const Tag& tag, EntityID entity)
        {
            if (GetEntity(entity)->RemoveTag(tag))
            {
                m_TaggedEntitiesToRemove.push_back({tag, entity});
                return true;
            }
            return false;
        }

        bool RemoveTag(const Tag& tag, EntityPointer<Types...>& pEntity) { return RemoveTag(tag, pEntity.get()); }

        const std::vector<EntityID>& AllEntities() const { return m_LivingEntities; }
        const std::vector<EntityID>& GetEntitiesByTag(const Tag& tag) const { return m_TaggedLists.at(tag); }

      private:
        void DestroyEntity(EntityID id)
        {
            assert(id < m_Entities.size());
            if (m_Entities[id].IsAlive())
            {
                m_Entities[id].Kill();
                m_ToDelete.push_back(m_Entities[id].ID());
            }
        }

        template <typename T>
        Memory::pool_pointer<T> CreateComponent()
        {
            auto& pool = std::get<Memory::PoolMemoryBlock<T>>(m_Pools);

            return std::move(pool.template CreateObject<T>(T()));
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
