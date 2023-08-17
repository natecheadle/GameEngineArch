#pragma once

#include <3D/Sprite.h>
#include <Entity.h>
#include <RigidBody2D.h>

namespace Ignosi::Examples::PhysicsTest
{
    class Ball : public Modules::ECS::Entity<Modules::Render::Sprite, Modules::Physics::RigidBody2D>
    {
        using BASE = Modules::ECS::Entity<Modules::Render::Sprite, Modules::Physics::RigidBody2D>;

      public:
        Ball(
            Modules::Memory::pool_pointer<Modules::Render::Sprite>       sprite,
            Modules::Memory::pool_pointer<Modules::Physics::RigidBody2D> rigidBody);

        Ball(Ball&& other);
        Ball& operator=(Ball&& other);

        ~Ball();

        Modules::Render::Sprite&       Sprite() { return BASE::Get<Modules::Render::Sprite>(); }
        const Modules::Render::Sprite& Sprite() const { return BASE::Get<Modules::Render::Sprite>(); }

        Modules::Physics::RigidBody2D&       RigidBody() { return BASE::Get<Modules::Physics::RigidBody2D>(); }
        const Modules::Physics::RigidBody2D& RigidBody() const { return BASE::Get<Modules::Physics::RigidBody2D>(); }

      private:
        void OnPositionUpdate(const Modules::Vector2<float>& value);
        void OnCollision(const Modules::Physics::RigidBody2D& other);
    };
} // namespace Ignosi::Examples::PhysicsTest
