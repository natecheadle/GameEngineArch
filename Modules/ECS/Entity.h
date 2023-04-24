#pragma once

#include "Component.h"

#include <UID.hpp>

namespace nate::Modules::ECS
{
    class Entity
    {
        std::uint64_t m_ID{UID<Entity>()};

      public:
        Entity()          = default;
        virtual ~Entity() = default;

        Entity(const Entity& other) = delete;
        Entity(Entity&& other)      = default;

        Entity& operator=(const Entity& other) = delete;
        Entity& operator=(Entity&& other)      = default;

        std::uint64_t ID() const { return m_ID; }

        template <class T>
        void Add(T val = T())
        {
        }

        template <class T>
        void Set(T val)
        {
        }

        template <class T>
        Component<T> Get()
        {
        }
    };
} // namespace nate::Modules::ECS
