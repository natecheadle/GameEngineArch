#pragma once

#include "Entity.h"

#include <PoolMemoryBlock.hpp>

#include <tuple>

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

        template <typename... ComponentTypes>
        Entity<ComponentTypes...> CreateEntity(ComponentTypes&&... args)
        {
            return Entity<ComponentTypes...>(CreatePoolObject(std::move(args))...);
        }

      private:
        template <typename T>
        Memory::pool_pointer<T> CreatePoolObject(T&& val)
        {
            auto& pool = std::get<Memory::PoolMemoryBlock<T>>(m_Pools);

            return pool.CreateObject<T>(std::move(val));
        }
    };
} // namespace nate::Modules::ECS
