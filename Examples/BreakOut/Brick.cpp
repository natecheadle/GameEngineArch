#include "Brick.h"

namespace nate::BreakOut
{
    Brick::Brick(
        Modules::Memory::pool_pointer<Modules::Render::Sprite>&&       sprite,
        Modules::Memory::pool_pointer<Modules::Physics::RigidBody2D>&& body)
        : BreakOutEntity(std::move(sprite), std::move(body))
    {
    }

    void Brick::OnCollision(const Modules::Physics::RigidBody2D& /* other */)
    {
        Sprite().Color({0.0, 0.0, 0.0});
    }
} // namespace nate::BreakOut
