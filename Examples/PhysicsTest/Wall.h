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

        Modules::Render::Sprite&       Sprite() { return BASE::Get<Modules::Render::Sprite>(); }
        const Modules::Render::Sprite& Sprite() const { return BASE::Get<Modules::Render::Sprite>(); }
    };
} // namespace Ignosi::Examples::PhysicsTest
