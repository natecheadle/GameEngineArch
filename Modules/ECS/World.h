#pragma once

#include "ComponentPool.h"

#include <UID.hpp>

namespace nate::Modules::ECS
{
    class World
    {

      public:
        World()          = default;
        virtual ~World() = default;

        World(const World& other) = delete;
        World(World&& other)      = default;

        World& operator=(const World& other) = delete;
        World& operator=(World&& other)      = default;
    };
} // namespace nate::Modules::ECS
