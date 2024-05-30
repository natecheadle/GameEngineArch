#pragma once

#include "IComponent.h"
#include "IEntity.h"

#include <chrono>

namespace Ignosi::Libraries::ECS
{
    class ISystem;

    class IWorld
    {
      public:
        virtual ~IWorld() = default;

        virtual void Update(std::chrono::milliseconds delta) = 0;

        virtual IEntity*       GetEntity(size_t id)       = 0;
        virtual const IEntity* GetEntity(size_t id) const = 0;

        virtual IComponent*       GetComponent(size_t entityID, size_t typeID)       = 0;
        virtual const IComponent* GetComponent(size_t entityID, size_t typeID) const = 0;

        virtual ISystem*       GetSystem(size_t typeID)       = 0;
        virtual const ISystem* GetSystem(size_t typeID) const = 0;
    };
} // namespace Ignosi::Libraries::ECS

#include "ISystem.h"
