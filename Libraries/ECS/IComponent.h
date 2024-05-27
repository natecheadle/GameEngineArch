#pragma once

#include <cstddef>

namespace Ignosi::Libraries::ECS
{
    class IComponent
    {
      public:
        virtual ~IComponent() = default;

        virtual size_t TypeID() const   = 0;
        virtual size_t EntityID() const = 0;
    };
} // namespace Ignosi::Libraries::ECS
