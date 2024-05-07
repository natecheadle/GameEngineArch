#pragma once

#include "PoolPointer.hpp"

#include <ObjectPool.hpp>

#include <tuple>
#include <utility>

namespace Ignosi::Libraries::ECS
{
    template <typename... Components>
    class World;

    template <typename... Components>
    class Entity
    {
        World<Components...>&                              m_World;
        std::tuple<Containers::PoolPointer<Components>...> m_Components;

        friend World<Components...>;

        Entity(World<Components...>& world)
            : m_World(world)
        {
        }

      public:
        ~Entity() = default;

        Entity(Entity&& other) noexcept            = default;
        Entity& operator=(Entity&& other) noexcept = default;

        template <typename T>
        void Initialize(Containers::PoolPointer<T>&& value)
        {
            std::get<Containers::PoolPointer<T>>(m_Components) = std::forward<Containers::PoolPointer<T>>(value);
        }

        template <typename T>
        void Clear()
        {
            std::get<Containers::PoolPointer<T>>(m_Components)->Reset();
        }

        template <typename T>
        void Set(const T& val)
        {
            *(std::get<Containers::PoolPointer<T>>(m_Components)) = val;
        }

        template <typename T>
        T& Get()
        {
            return *(std::get<Containers::PoolPointer<T>>(m_Components));
        }

        template <typename T>
        const T& Get() const
        {
            return *(std::get<Containers::PoolPointer<T>>(m_Components));
        }

        template <typename T>
        bool Has() const
        {
            return std::get<Containers::PoolPointer<T>>(m_Components)->IsValid();
        }
    };
} // namespace Ignosi::Libraries::ECS
