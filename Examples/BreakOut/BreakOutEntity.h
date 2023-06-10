#pragma once

#include "Physics/RigidBody2D.h"

#include <3D/Sprite.h>
#include <Entity.h>

namespace nate::BreakOut
{
    using BreakOutEntity = Modules::ECS::Entity<Modules::Render::Sprite, Modules::Physics::RigidBody2D>;
}
