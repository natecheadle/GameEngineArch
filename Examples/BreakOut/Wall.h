#pragma once

#include "BreakOutEntity.h"
#include "Entity.h"
#include "Physics/RigidBody2D.h"

#include <utility>

namespace nate::BreakOut
{
    class Wall : public Modules::ECS::Entity<Modules::Physics::RigidBody2D>
    {
        using BASE = Modules::ECS::Entity<Modules::Physics::RigidBody2D>;

      public:
        Wall(Modules::Memory::pool_pointer<Modules::Physics::RigidBody2D>&& body)
            : BASE(std::move(body))
        {
        }

        Wall(Wall&& other)            = default;
        Wall& operator=(Wall&& other) = default;

        void                           Position(const Modules::Vector2<float>& pos) { Body().Position(pos); }
        const Modules::Vector2<float>& Position() const { return Body().Position(); }

        const Modules::Vector2<float>& HitBox() const { return Body().HitBox(); }
        void                           HitBox(const Modules::Vector2<float>& val) { Body().HitBox(val); }

      private:
        Modules::Physics::RigidBody2D&       Body() { return BASE::Get<Modules::Physics::RigidBody2D>(); }
        const Modules::Physics::RigidBody2D& Body() const { return BASE::Get<Modules::Physics::RigidBody2D>(); }
    };
} // namespace nate::BreakOut
