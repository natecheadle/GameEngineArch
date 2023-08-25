#pragma once

#include "Entity.h"
#include "System.h"
#include "Tag.h"

#include <MutexProtected.hpp>
#include <PoolMemoryBlock.hpp>

#include <cstddef>
#include <mutex>
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
        Memory::PoolMemoryBlock<Entity<Types...>>     m_EntityPool;

      public:
        World()          = default;
        virtual ~World() = default;

        World(const World& other) = delete;
        World(World&& other)      = default;

        World& operator=(const World& other) = delete;
        World& operator=(World&& other)      = default;

        template <class... Components>
        Memory::pool_pointer<Entity<Types...>> CreateEntity()
        {
            Memory::pool_pointer<Entity<Types...>> pEntity = m_EntityPool.CreateObject();

            (pEntity->template InitializeComponent<Components>(std::move(CreateComponent<Components>())), ...);
            return std::move(pEntity);
        }

        template <typename T, typename... ComponentTypes>
        std::unique_ptr<T> CreateSystem()
        {
            static_assert(std::is_base_of_v<System<ComponentTypes...>, T>, "T must be derive from System.");
            return std::make_unique<T>(&(std::get<Memory::PoolMemoryBlock<ComponentTypes>>(m_Pools))...);
        }
        template <typename T>
        Memory::pool_pointer<T> CreateComponent()
        {
            auto& pool = std::get<Memory::PoolMemoryBlock<T>>(m_Pools);

            return std::move(pool.template CreateObject<T>(T()));
        }
    };
} // namespace Ignosi::Modules::ECS
