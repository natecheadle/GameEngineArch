#pragma once

#include <string>

namespace Ignosi::Libraries::ECS
{
    class IResource
    {
      public:
        IResource()          = default;
        virtual ~IResource() = default;

        virtual const std::string& ID() const = 0;
    };
} // namespace Ignosi::Libraries::ECS
