#pragma once

#include "IWorld.h"

#include <chrono>

namespace Ignosi::Libraries::ECS
{

    class ISystem
    {
      public:
        virtual ~ISystem() = default;

        virtual const IWorld* Parent() const                          = 0;
        virtual size_t        ID() const                              = 0;
        virtual void          Update(std::chrono::milliseconds delta) = 0;
    };
} // namespace Ignosi::Libraries::ECS
