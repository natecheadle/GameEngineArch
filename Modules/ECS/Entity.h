#pragma once

#include "Tag.h"

#include <PoolMemoryBlock.hpp>

#include <atomic>
#include <cassert>
#include <cstddef>
#include <limits>
#include <memory>
#include <tuple>
#include <vector>

namespace Ignosi::Modules::ECS
{
    template <typename... ComponentTypes>
    class World;

    template <typename... ComponentTypes>
    class Entity final
    {
        friend class World<ComponentTypes...>;

        std::uint64_t     m_ID{std::numeric_limits<std::uint64_t>::max()};
        std::atomic<bool> m_IsAlive{false};
        std::vector<Tag>  m_Tags;

        std::tuple<Memory::pool_pointer<ComponentTypes>...> m_Components;

        void Kill() { m_IsAlive = false; }

        void Revive()
        {
            // Doing the "reset" during revive to prevent iterator invalidation during Kill().
            (std::get<Memory::pool_pointer<ComponentTypes>>(m_Components).reset(), ...);
            m_IsAlive = true;
        }

        Entity(std::uint64_t id)
            : m_ID(id)
        {
        }

      public:
        Entity() = default;

        virtual ~Entity() = default;

        Entity(const Entity& other) = delete;
        Entity(Entity&& other)
        {
            m_Tags = std::move(other.m_Tags);
            m_ID   = other.m_ID;
            ((std::get<Memory::pool_pointer<ComponentTypes>>(m_Components) =
                  std::move(std::get<Memory::pool_pointer<ComponentTypes>>(other.m_Components))),
             ...);

            other.m_IsAlive = false;
            other.m_ID      = std::numeric_limits<std::uint64_t>::max();
        }

        Entity& operator=(const Entity& other) = delete;
        Entity& operator=(Entity&& other)
        {
            m_Tags = std::move(other.m_Tags);
            m_ID   = other.m_ID;
            ((std::get<Memory::pool_pointer<ComponentTypes>>(m_Components) =
                  std::move(std::get<Memory::pool_pointer<ComponentTypes>>(other.m_Components))),
             ...);

            other.m_IsAlive = false;
            other.m_ID      = std::numeric_limits<std::uint64_t>::max();

            return *this;
        }

        std::uint64_t ID() const { return m_ID; }

        bool IsAlive() const { return m_IsAlive; }

        template <typename T>
        void InitializeComponent(Memory::pool_pointer<T>&& value)
        {
            std::get<Memory::pool_pointer<T>>(m_Components) = std::move(value);
        }

        template <typename T>
        void ClearComponent()
        {
            std::get<Memory::pool_pointer<T>>(m_Components).reset();
        }

        template <typename T>
        void Set(const T& val)
        {
            auto& pComponent = std::get<Memory::pool_pointer<T>>(m_Components);
            *pComponent      = val;
        }

        template <typename T>
        const T& Get() const
        {
            auto& pComponent = std::get<Memory::pool_pointer<T>>(m_Components);
            return *pComponent;
        }

        template <typename T>
        T& Get()
        {
            auto& pComponent = std::get<Memory::pool_pointer<T>>(m_Components);
            return *pComponent;
        }

        template <typename T>
        T* GetPointer()
        {
            auto& pComponent = std::get<Memory::pool_pointer<T>>(m_Components);
            return pComponent.get();
        }

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
