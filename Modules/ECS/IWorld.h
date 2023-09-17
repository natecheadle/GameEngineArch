#pragma once

#include "IEntity.h"
#include "ISystem.h"
#include "ResourceManager.h"

namespace Ignosi::Modules::ECS
{
    class IWorld
    {
      public:
        virtual ~IWorld() = default;

        virtual void Update(double dt) = 0;

        virtual const std::vector<EntityID>& AllEntities() const                        = 0;
        virtual const std::vector<EntityID>& GetEntitiesByTag(const Tag& tag) const     = 0;
        virtual IEntity*                     GetEntity(EntityID id)                     = 0;
        virtual const IEntity*               GetEntity(EntityID id) const               = 0;
        virtual ISystem*                     GetSystem(SystemID id)                     = 0;
        virtual bool                         RemoveTag(const Tag& tag, EntityID entity) = 0;
        virtual bool                         AddTag(const Tag& tag, EntityID entity)    = 0;
        virtual const ResourceManager&       Resources() const                          = 0;
        virtual ResourceManager&             Resources()                                = 0;
    };
} // namespace Ignosi::Modules::ECS
