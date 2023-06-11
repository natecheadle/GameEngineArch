#pragma once

#include "BreakOutEntity.h"

namespace nate::BreakOut
{
    class Paddle : public BreakOutEntity
    {
      public:
        Paddle(
            Modules::Memory::pool_pointer<Modules::Render::Sprite>&&       sprite,
            Modules::Memory::pool_pointer<Modules::Physics::RigidBody2D>&& body);

        void                           Position(const Modules::Vector2<float>& pos);
        const Modules::Vector2<float>& Position() const { return Body().Position(); }

      private:
        Modules::Render::Sprite&       Sprite() { return BreakOutEntity::Get<Modules::Render::Sprite>(); }
        const Modules::Render::Sprite& Sprite() const { return BreakOutEntity::Get<Modules::Render::Sprite>(); }

        Modules::Physics::RigidBody2D&       Body() { return BreakOutEntity::Get<Modules::Physics::RigidBody2D>(); }
        const Modules::Physics::RigidBody2D& Body() const
        {
            return BreakOutEntity::Get<Modules::Physics::RigidBody2D>();
        }
    };
} // namespace nate::BreakOut
