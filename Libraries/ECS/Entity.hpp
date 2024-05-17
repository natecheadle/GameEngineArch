#pragma once

#include "PoolPointer.hpp"

#include <Component.hpp>
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
        World<Components...>&                                         m_World;
        size_t                                                        m_ID;
        std::tuple<Containers::PoolPointer<Component<Components>>...> m_Components;

        friend World<Components...>;

        Entity(World<Components...>& world)
            : m_World(world)
        {
        }

      public:
        ~Entity() = default;

        Entity(Entity&& other) noexcept            = default;
        Entity& operator=(Entity&& other) noexcept = default;

        size_t ID() const { return m_ID; }

        template <typename T>
        void Clear()
        {
            std::get<Containers::PoolPointer<Component<T>>>(m_Components)->Reset();
        }

        template <typename T>
        void Set(const T& val)
        {
            if (!Has<T>())
            {
                std::get<Containers::PoolPointer<Component<T>>>(m_Components) = std::move(m_World.template CreateComponent<T>(*this, val));
            }
            else
            {
                std::get<Containers::PoolPointer<Component<T>>>(m_Components)->Data() = val;
            }
        }

        template <typename T>
        T& Get()
        {
            return std::get<Containers::PoolPointer<Component<T>>>(m_Components)->Data();
        }

        template <typename T>
        const T& Get() const
        {
            return std::get<Containers::PoolPointer<Component<T>>>(m_Components)->Data();
        }

        template <typename T>
        bool Has() const
        {
            return std::get<Containers::PoolPointer<Component<T>>>(m_Components).IsValid();
        }
    };
} // namespace Ignosi::Libraries::ECS

#include "World.hpp"
