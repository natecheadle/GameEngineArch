#pragma once

#include <string_view>
#include <tuple>

namespace Ignosi::Modules::ECS
{
    class IWorld;
    using SystemID = std::uint32_t;

    class ISystem
    {
      public:
        virtual ~ISystem() = default;

        virtual SystemID ID() const                                   = 0;
        virtual void     Initialize(std::uint32_t id, IWorld* pWorld) = 0;

        virtual std::string_view Name() const      = 0;
        virtual std::uint32_t    Priority() const  = 0;
        virtual void             Update(double dt) = 0;
    };
} // namespace Ignosi::Modules::ECS
