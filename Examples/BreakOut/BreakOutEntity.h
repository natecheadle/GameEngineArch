#pragma once

#include "Entity.h"

#include <3D/Mesh3D.h>
#include <3D/Sprite.h>
#include <CustomEntity.h>
#include <RigidBody2D.h>

namespace Ignosi::BreakOut
{
    using BreakOutWorld = Modules::ECS::
        World<Modules::Render::Mesh3D, Modules::Render::Sprite, Modules::Physics::RigidBody2D, Modules::Physics::KinematicData>;

    using CustomBreakOutEntity = Modules::ECS::
        CustomEntity<Modules::Render::Mesh3D, Modules::Render::Sprite, Modules::Physics::RigidBody2D, Modules::Physics::KinematicData>;

    using BreakOutEntity = Modules::ECS::
        Entity<Modules::Render::Mesh3D, Modules::Render::Sprite, Modules::Physics::RigidBody2D, Modules::Physics::KinematicData>;

    using BreakOutEntityPointer = Modules::ECS::
        EntityPointer<Modules::Render::Mesh3D, Modules::Render::Sprite, Modules::Physics::RigidBody2D, Modules::Physics::KinematicData>;

} // namespace Ignosi::BreakOut
