#pragma once

#include "ClassID.hpp"
#include "Tag.h"

#include <functional>
#include <vector>

namespace Ignosi::Modules::ECS
{
    class EntityID : public ClassID<EntityID>
    {
      public:
        EntityID() = default;
        EntityID(size_t id)
            : ClassID<EntityID>(id)
        {
        }
    };

    class IEntity
    {
      public:
        virtual EntityID                ID() const                                           = 0;
        virtual bool                    IsAlive() const                                      = 0;
        virtual void                    Update(double dt)                                    = 0;
        virtual void                    AttachOnUpdate(std::function<void(double)> callback) = 0;
        virtual const std::vector<Tag>& Tags() const                                         = 0;
        virtual bool                    HasTag(const Tag& tag) const                         = 0;
    };
} // namespace Ignosi::Modules::ECS
