#pragma once

#include "Component.hpp"
#include "ECSObject.hpp"
#include "IEntity.h"

#include <TupleElementIndex.hpp>

#include <tuple>
#include <utility>
#include <array>

namespace Ignosi::Libraries::ECS
{
    template <typename... Components>
    class World;

    template <typename... COMPONENTS>
    class Entity : public IEntity
    {
        World<COMPONENTS...>&                           m_World;
        size_t                                          m_ID;
        std::tuple<ECSObject<Component<COMPONENTS>>...> m_Components;
        std::array<IComponent*, sizeof...(COMPONENTS)>  m_ComponentLookup;

        friend World<COMPONENTS...>;

        Entity(World<COMPONENTS...>& world)
            : m_World(world)
        {
        }

      public:
        ~Entity() = default;

        Entity(Entity&& other) noexcept            = default;
        Entity& operator=(Entity&& other) noexcept = default;

        size_t ID() const override { return m_ID; }

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
                std::get<ECSObject<Component<T>>>(m_Components) = std::move(m_World.template CreateComponent<T>(*this, val));
            }
            else
            {
                std::get<ECSObject<Component<T>>>(m_Components)->Data() = val;
            }

            size_t elementIndex = TupleElementIndex<ECSObject<Component<T>>, std::tuple<ECSObject<Component<COMPONENTS>>...>>::value;
            m_ComponentLookup[elementIndex] = std::get<ECSObject<Component<T>>>(m_Components).Get();
        }

        template <typename T>
        T& Get()
        {
            return std::get<ECSObject<Component<T>>>(m_Components)->Data();
        }

        IComponent*       Get(size_t id) override { return m_ComponentLookup.at(id); }
        const IComponent* Get(size_t id) const override { return m_ComponentLookup.at(id); }

        template <typename T>
        const T& Get() const
        {
            return std::get<ECSObject<Component<T>>>(m_Components)->Data();
        }

        template <typename T>
        bool Has() const
        {
            return std::get<ECSObject<Component<T>>>(m_Components).IsAlive();
        }
    };
} // namespace Ignosi::Libraries::ECS

#include "World.hpp"
