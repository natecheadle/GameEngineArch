#pragma once

#include "ISystem.h"

#include <PoolMemoryBlock.hpp>
#include <UID.hpp>

#include <tuple>

namespace nate::Modules::ECS
{
    template <typename... Types>
    class System : ISystem
    {
        std::tuple<Memory::PoolMemoryBlock<Types>*...> m_Pools;

      protected:
        System(Memory::PoolMemoryBlock<Types>*... args)
            : m_Pools(std::move(args)...)
        {
        }

      public:
        virtual ~System() = default;

        System(const System& other) = delete;
        System(System&& other)      = default;

        System& operator=(const System& other) = delete;
        System& operator=(System&& other)      = default;

      protected:
        template <typename T>
        Memory::PoolMemoryBlock<T>& GetPool()
        {
            return *(std::get<Memory::PoolMemoryBlock<T>*>(m_Pools));
        }
    };
} // namespace nate::Modules::ECS
