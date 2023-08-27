#pragma once

#include "Entity.h"
#include "EntityPointer.h"
#include "System.h"
#include "Tag.h"

#include <PoolMemoryBlock.hpp>
#include <sys/_types/_size_t.h>

#include <algorithm>
#include <atomic>
#include <cstddef>
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

        std::vector<std::uint64_t> m_LivingEntities;
        std::queue<std::uint64_t>  m_FreedEntities;
        std::vector<std::uint64_t> m_ToDelete;
        std::vector<std::uint64_t> m_ToAdd;

        std::map<Tag, std::vector<std::uint64_t>>  m_TaggedLists;
        std::vector<std::pair<Tag, std::uint64_t>> m_TaggedEntitiesToAdd;
        std::vector<std::pair<Tag, std::uint64_t>> m_TaggedEntitiesToRemove;

        friend EntityPointer<Types...>;

      public:
        World()          = default;
        virtual ~World() = default;

        World(const World& other) = delete;
        World(World&& other)      = default;

        World& operator=(const World& other) = delete;
        World& operator=(World&& other)      = default;

        Entity<Types...>& GetEntity(size_t id)
        {
            assert(id < m_Entities.size());
            assert(std::find(m_LivingEntities.begin(), m_LivingEntities.end(), id) != m_LivingEntities.end());
            return m_Entities[id];
        }

        const Entity<Types...>& GetEntity(size_t id) const
        {
            assert(id < m_Entities.size());
            assert(std::find(m_LivingEntities.begin(), m_LivingEntities.end(), id) != m_LivingEntities.end());
            return m_Entities[id];
        }

        template <class... Components>
        EntityPointer<Types...> CreateEntity()
        {
            Entity<Types...>* pEntity{nullptr};
            if (!m_FreedEntities.empty())
            {
                std::uint64_t newId = m_FreedEntities.front();
                m_FreedEntities.pop();
                pEntity = &m_Entities[newId];
            }
            else
            {
                std::uint64_t newID = m_Entities.size();
                m_Entities.push_back(std::move(Entity<Types...>(newID)));
                pEntity = &m_Entities[m_Entities.size() - 1];
            }

            m_ToAdd.push_back(pEntity->ID());

            pEntity->Revive();
            (pEntity->template InitializeComponent<Components>(std::move(CreateComponent<Components>())), ...);

            return {pEntity->ID(), this};
        }

        void Update()
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

        template <typename T, typename... ComponentTypes>
        std::unique_ptr<T> CreateSystem()
        {
            static_assert(std::is_base_of_v<System<ComponentTypes...>, T>, "T must be derive from System.");
            return std::make_unique<T>(&(std::get<Memory::PoolMemoryBlock<ComponentTypes>>(m_Pools))...);
        }

        bool AddTag(const Tag& tag, std::uint64_t entity)
        {
            if (GetEntity(entity).AddTag(tag))
            {
                m_TaggedEntitiesToAdd.push_back({tag, entity});
                return true;
            }
            return false;
        }

        bool AddTag(const Tag& tag, EntityPointer<Types...>& pEntity) { return AddTag(tag, pEntity->ID()); }

        bool RemoveTag(const Tag& tag, std::uint64_t entity)
        {
            if (GetEntity(entity)->RemoveTag(tag))
            {
                m_TaggedEntitiesToRemove.push_back({tag, entity});
                return true;
            }
            return false;
        }

        bool RemoveTag(const Tag& tag, EntityPointer<Types...>& pEntity) { return RemoveTag(tag, pEntity.get()); }

        const std::vector<std::uint64_t>& AllEntities() const { return m_LivingEntities; }
        const std::vector<std::uint64_t>& GetEntitiesByTag(const Tag& tag) const { return m_TaggedLists.at(tag); }

      private:
        void DestroyEntity(std::uint64_t id)
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
    };
} // namespace Ignosi::Modules::ECS
