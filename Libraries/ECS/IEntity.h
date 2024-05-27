#pragma once

#include "IComponent.h"

#include <cstddef>

namespace Ignosi::Libraries::ECS
{
    class IEntity
    {
      public:
        virtual ~IEntity() = default;

        virtual size_t ID() const = 0;

        virtual IComponent*       Get(size_t typeID)       = 0;
        virtual const IComponent* Get(size_t typeID) const = 0;
    };
} // namespace Ignosi::Libraries::ECS
