#pragma once

#include "ClassID.hpp"
#include "IEntity.h"
#include "Tag.h"

#include <string_view>
#include <tuple>

namespace Ignosi::Modules::ECS
{
    class IWorld;

    class SystemID : public ClassID<SystemID>
    {
      public:
        SystemID() = default;
        SystemID(size_t id)
            : ClassID<SystemID>(id)
        {
        }
    };

    class ISystem
    {
      public:
        virtual ~ISystem() = default;

        virtual SystemID ID() const                              = 0;
        virtual void     Initialize(SystemID id, IWorld* pWorld) = 0;

        virtual std::string_view Name() const                                         = 0;
        virtual const Tag&       Tag() const                                          = 0;
        virtual std::uint32_t    Priority() const                                     = 0;
        virtual void             Update(double dt)                                    = 0;
        virtual bool             EntityHasNecessaryComponents(IEntity* pEntity) const = 0;
    };
} // namespace Ignosi::Modules::ECS
