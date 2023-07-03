#pragma once

#include <cstdint>

namespace Ignosi::Modules::ECS
{
    class IEntity
    {
      public:
        virtual ~IEntity()               = default;
        virtual std::uint64_t ID() const = 0;
    };
} // namespace Ignosi::Modules::ECS
