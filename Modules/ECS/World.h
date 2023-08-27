#pragma once

#include "Entity.h"
#include "EntityPointer.h"
#include "System.h"
#include "Tag.h"

#include <PoolMemoryBlock.hpp>

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

        std::vector<Entity<Types...>*> m_LivingEntities;
        std::queue<std::uint64_t>      m_FreedEntities;
        std::vector<std::uint64_t>     m_ToDelete;
        std::vector<std::uint64_t>     m_ToAdd;

        friend EntityPointer<Types...>;

      public:
        World()          = default;
        virtual ~World() = default;

        World(const World& other) = delete;
        World(World&& other)      = default;

        World& operator=(const World& other) = delete;
        World& operator=(World&& other)      = default;

        template <class... Components>
        EntityPointer<Types...> CreateEntity()
        {
            Entity<Types...>* pEntity{nullptr};
            if (!m_FreedEntities.empty())
            {
                std::uint64_t newId = m_FreedEntities.front();
                m_FreedEntities.pop();

                m_Entities[newId].Revive();
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
            for (auto destroyID : m_ToDelete)
            {
                auto it = std::lower_bound(
                    m_LivingEntities.begin(),
                    m_LivingEntities.end(),
                    destroyID,
                    [](const Entity<Types...>* pEntity, std::uint64_t id) { return pEntity->ID() < id; });

                assert(it != m_LivingEntities.end());
                m_LivingEntities.erase(it);
                m_FreedEntities.push(destroyID);
            }

            for (auto addID : m_ToAdd)
            {
                m_LivingEntities.push_back(&m_Entities[addID]);
            }

            if (!m_ToAdd.empty())
            {
                std::sort(m_LivingEntities.begin(), m_LivingEntities.end());
            }

            m_ToAdd.clear();
            m_ToDelete.clear();
        }

        template <typename T, typename... ComponentTypes>
        std::unique_ptr<T> CreateSystem()
        {
            static_assert(std::is_base_of_v<System<ComponentTypes...>, T>, "T must be derive from System.");
            return std::make_unique<T>(&(std::get<Memory::PoolMemoryBlock<ComponentTypes>>(m_Pools))...);
        }

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
