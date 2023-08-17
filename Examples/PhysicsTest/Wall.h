#pragma once

#include <3D/Sprite.h>
#include <Entity.h>
#include <RigidBody2D.h>

namespace Ignosi::Examples::PhysicsTest
{
    class Wall : public Modules::ECS::Entity<Modules::Render::Sprite, Modules::Physics::RigidBody2D>
    {
        using BASE = Modules::ECS::Entity<Modules::Render::Sprite, Modules::Physics::RigidBody2D>;

      public:
        Wall(
            Modules::Memory::pool_pointer<Modules::Render::Sprite>       sprite,
            Modules::Memory::pool_pointer<Modules::Physics::RigidBody2D> rigidBody);

        Modules::Render::Sprite&       Sprite() { return BASE::Get<Modules::Render::Sprite>(); }
        const Modules::Render::Sprite& Sprite() const { return BASE::Get<Modules::Render::Sprite>(); }

        Modules::Physics::RigidBody2D&       RigidBody() { return BASE::Get<Modules::Physics::RigidBody2D>(); }
        const Modules::Physics::RigidBody2D& RigidBody() const { return BASE::Get<Modules::Physics::RigidBody2D>(); }
    };
} // namespace Ignosi::Examples::PhysicsTest
