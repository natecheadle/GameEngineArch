#pragma once

#include "ComponentPool.h"
#include "IEntity.h"
#include "Tag.h"

#include <PoolMemoryBlock.hpp>

#include <atomic>
#include <cassert>
#include <cstddef>
#include <functional>
#include <limits>
#include <memory>
#include <tuple>
#include <vector>

namespace Ignosi::Modules::ECS
{
    template <typename... ComponentTypes>
    class World;

    template <typename... ComponentTypes>
    class Entity : public IEntity
    {
        friend class World<ComponentTypes...>;

        EntityID                    m_ID{EntityID::RESET_VAL};
        bool                        m_IsAlive{false};
        std::vector<Tag>            m_Tags;
        std::function<void(double)> m_OnUpdate;

        std::tuple<ComponentPointer<ComponentTypes>...> m_Components;

        void Kill() { m_IsAlive = false; }

        void Revive()
        {
            // Doing the "reset" during revive to prevent iterator invalidation during Kill().
            (std::get<ComponentPointer<ComponentTypes>>(m_Components).reset(), ...);
            m_Tags.clear();

            m_IsAlive = true;
        }

        Entity(EntityID id)
            : m_ID(id)
        {
        }

      public:
        Entity() = default;

        virtual ~Entity() = default;

        Entity(const Entity& other) = delete;
        Entity(Entity&& other) noexcept
        {
            m_Tags    = std::move(other.m_Tags);
            m_ID      = other.m_ID;
            m_IsAlive = other.m_IsAlive;
            ((std::get<ComponentPointer<ComponentTypes>>(m_Components) =
                  std::move(std::get<ComponentPointer<ComponentTypes>>(other.m_Components))),
             ...);

            other.m_IsAlive = false;
            other.m_ID.Reset();
            other.m_Tags.clear();
        }

        Entity& operator=(const Entity& other) = delete;
        Entity& operator=(Entity&& other)
        {
            m_Tags    = std::move(other.m_Tags);
            m_ID      = other.m_ID;
            m_IsAlive = other.m_IsAlive;
            ((std::get<ComponentPointer<ComponentTypes>>(m_Components) =
                  std::move(std::get<ComponentPointer<ComponentTypes>>(other.m_Components))),
             ...);

            other.m_IsAlive = false;
            other.m_ID.Reset();
            other.m_Tags.clear();

            return *this;
        }

        EntityID ID() const override { return m_ID; }
        bool     IsAlive() const override { return m_IsAlive; }
        void     Update(double dt) override
        {
            if (m_OnUpdate)
                m_OnUpdate(dt);
        }
        void                    AttachOnUpdate(std::function<void(double)> callback) override { m_OnUpdate = std::move(callback); }
        const std::vector<Tag>& Tags() const override { return m_Tags; }

        template <typename T>
        void InitializeComponent(ComponentPointer<T>&& value)
        {
            std::get<ComponentPointer<T>>(m_Components) = std::move(value);
        }

        template <typename T>
        void ClearComponent()
        {
            std::get<ComponentPointer<T>>(m_Components).reset();
        }

        template <typename T>
        void Set(const T& val)
        {
            auto& pComponent = std::get<ComponentPointer<T>>(m_Components);
            *pComponent      = val;
        }

        template <typename T>
        const T* Get() const
        {
            auto& pComponent = std::get<ComponentPointer<T>>(m_Components);
            return pComponent.get();
        }

        template <typename T>
        T* Get()
        {
            auto& pComponent = std::get<ComponentPointer<T>>(m_Components);
            return pComponent.get();
        }

      private:
        bool AddTag(const Tag& value)
        {

            if (std::find(m_Tags.begin(), m_Tags.end(), value) == m_Tags.end())
            {
                m_Tags.push_back(value);
                return true;
            }

            return false;
        }

        bool RemoveTag(const Tag& value)
        {

            auto it = std::find(m_Tags.begin(), m_Tags.end(), value);
            if (it != m_Tags.end())
            {
                m_Tags.erase(it);
            }

            return false;
        }
    };
} // namespace Ignosi::Modules::ECS
