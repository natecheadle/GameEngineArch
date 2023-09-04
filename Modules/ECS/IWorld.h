#pragma once

#include "IEntity.h"

namespace Ignosi::Modules::ECS
{
    class IWorld
    {
      public:
        virtual void                         Update(double dt)                          = 0;

        virtual const std::vector<EntityID>& AllEntities() const                        = 0;
        virtual const std::vector<EntityID>& GetEntitiesByTag(const Tag& tag) const     = 0;
        virtual IEntity*                     GetEntity(EntityID id)                     = 0;
        virtual const IEntity*               GetEntity(EntityID id) const               = 0;
        virtual bool                         RemoveTag(const Tag& tag, EntityID entity) = 0;
        virtual bool                         AddTag(const Tag& tag, EntityID entity)    = 0;
    };
} // namespace Ignosi::Modules::ECS
