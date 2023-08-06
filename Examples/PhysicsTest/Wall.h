#pragma once

#include <3D/Sprite.h>
#include <Entity.h>
#include <RigidBody2D.h>

namespace Ignosi::Examples::PhysicsTest
{
    class Wall : Modules::ECS::Entity<Modules::Render::Sprite, Modules::Physics::RigidBody2D>
    {
        using BASE = Modules::ECS::Entity<Modules::Render::Sprite, Modules::Physics::RigidBody2D>;

      public:
        Wall(
            Modules::Memory::pool_pointer<Modules::Render::Sprite>       sprite,
            Modules::Memory::pool_pointer<Modules::Physics::RigidBody2D> rigidBody);
    };
} // namespace Ignosi::Examples::PhysicsTest
