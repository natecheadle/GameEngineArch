#pragma once

#include "Entity.h"
#include "System.h"

#include <PoolMemoryBlock.hpp>

#include <tuple>
#include <type_traits>

namespace nate::Modules::ECS
{
    template <typename... Types>
    class World
    {
        std::tuple<Memory::PoolMemoryBlock<Types>...> m_Pools;

      public:
        World()          = default;
        virtual ~World() = default;

        World(const World& other) = delete;
        World(World&& other)      = default;

        World& operator=(const World& other) = delete;
        World& operator=(World&& other)      = default;

        template <typename T, typename... ComponentTypes>
        std::unique_ptr<T> CreateEntity(ComponentTypes&&... args)
        {
            static_assert(std::is_base_of_v<Entity<ComponentTypes...>, T>, "T must be derive from Entity.");
            return std::make_unique<T>(CreatePoolObject(std::move(args))...);
        }

        template <typename T, typename... ComponentTypes>
        std::unique_ptr<T> CreateSystem()
        {
            static_assert(std::is_base_of_v<System<ComponentTypes...>, T>, "T must be derive from System.");
            return std::make_unique<T>(&(std::get<Memory::PoolMemoryBlock<ComponentTypes>>(m_Pools))...);
        }

      private:
        template <typename T>
        Memory::pool_pointer<T> CreatePoolObject(T&& val)
        {
            auto& pool = std::get<Memory::PoolMemoryBlock<T>>(m_Pools);

            return pool.template CreateObject<T>(std::forward<T>(val));
        }
    };
} // namespace nate::Modules::ECS
