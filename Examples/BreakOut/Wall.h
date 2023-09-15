#pragma once

#include "BreakOutEntity.h"

#include <World.h>

#include <utility>

namespace Ignosi::BreakOut
{
    class Wall : CustomBreakOutEntity
    {
      public:
        Wall(BreakOutEntityPointer&& entity)
            : CustomBreakOutEntity(std::move(entity))
        {
            World()->AddComponent<Modules::Physics::KinematicData>(Entity());
            World()->AddComponent<Modules::Physics::RigidBody2D>(Entity(), GetComponent<Modules::Physics::KinematicData>());
        }

        Wall(Wall&& other)            = default;
        Wall& operator=(Wall&& other) = default;

        Modules::ECS::WeakComponentPointer<Modules::Physics::KinematicData> KinematicData() const
        {
            return GetComponent<Modules::Physics::KinematicData>();
        }

        Modules::ECS::WeakComponentPointer<Modules::Physics::RigidBody2D> Body() const
        {
            return GetComponent<Modules::Physics::RigidBody2D>();
        }

      protected:
        void OnUpdate(double dt) override {}
    };
} // namespace Ignosi::BreakOut
