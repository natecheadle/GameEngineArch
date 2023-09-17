#pragma once

#include "BreakOutEntity.h"

#include <LinearAlgebra/Vector2.hpp>
#include <World.h>

#include <utility>

namespace Ignosi::BreakOut
{
    class Wall : CustomBreakOutEntity
    {
      public:
        Wall(BreakOutWorld& world, Modules::Vector2<float> size, Modules::Vector2<float> position);

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
