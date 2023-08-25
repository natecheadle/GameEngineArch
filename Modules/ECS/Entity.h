#pragma once

#include "Tag.h"

#include <PoolMemoryBlock.hpp>
#include <UID.hpp>

#include <atomic>
#include <cassert>
#include <cstddef>
#include <memory>
#include <tuple>
#include <vector>

namespace Ignosi::Modules::ECS
{
    template <typename... Types>
    class World;

    template <typename... ComponentTypes>
    class Entity final
    {

        std::uint64_t     m_ID{UID()};
        std::atomic<bool> m_IsAlive{true};
        std::vector<Tag>  m_Tags;

        std::tuple<Memory::pool_pointer<ComponentTypes>...> m_Components;

      public:
        Entity()          = default;
        virtual ~Entity() = default;

        Entity(const Entity& other) = delete;
        Entity(Entity&& other)      = default;

        Entity& operator=(const Entity& other) = delete;
        Entity& operator=(Entity&& other)      = default;

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
            if (!value.IsValid())
                return false;

            if (std::find(m_Tags.begin(), m_Tags.end(), value) == m_Tags.end())
            {
                m_Tags.push_back(value);
                return true;
            }

            return false;
        }

        bool RemoveTag(const Tag& value)
        {
            if (!value.IsValid())
                return false;

            auto it = std::find(m_Tags.begin(), m_Tags.end(), value);
            if (it != m_Tags.end())
            {
                m_Tags.erase(it);
            }

            return false;
        }
    };
} // namespace Ignosi::Modules::ECS
