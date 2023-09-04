#pragma once

#include "ClassID.hpp"

namespace Ignosi::Modules::ECS
{
    class ComponentID : public ClassID<ComponentID>
    {
      public:
        ComponentID() = default;
        ComponentID(size_t id)
            : ClassID<ComponentID>(id)
        {
        }
    };
} // namespace Ignosi::Modules::ECS