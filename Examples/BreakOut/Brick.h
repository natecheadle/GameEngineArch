#pragma once

#include "BreakOutEntity.h"
#include "Physics/RigidBody2D.h"

#include <utility>

namespace nate::BreakOut
{
    enum class BrickType
    {
        None     = 0,
        Solid    = 1,
        BabyBlue = 2,
        Green    = 3,
    };

    class Brick : public BreakOutEntity
    {
        BrickType m_Type;

      public:
        Brick(
            Modules::Memory::pool_pointer<Modules::Render::Sprite>&&       sprite,
            Modules::Memory::pool_pointer<Modules::Physics::RigidBody2D>&& body);

        BrickType Type() const { return m_Type; }
        void      Type(BrickType val) { m_Type = val; }

        void                           Position(const Modules::Vector2<float>& pos);
        const Modules::Vector2<float>& Position() const { return Body().Position(); }

        void Draw(Modules::Render::ShaderProgram* pProgram) { Sprite().Draw(pProgram); }

      private:
        Modules::Render::Sprite&       Sprite() { return BreakOutEntity::Get<Modules::Render::Sprite>(); }
        const Modules::Render::Sprite& Sprite() const { return BreakOutEntity::Get<Modules::Render::Sprite>(); }

        Modules::Physics::RigidBody2D&       Body() { return BreakOutEntity::Get<Modules::Physics::RigidBody2D>(); }
        const Modules::Physics::RigidBody2D& Body() const
        {
            return BreakOutEntity::Get<Modules::Physics::RigidBody2D>();
        }

        void OnCollision(const Modules::Physics::RigidBody2D& other);
    };
} // namespace nate::BreakOut
