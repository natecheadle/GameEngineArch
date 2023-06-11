#include "Paddle.h"

namespace nate::BreakOut
{
    Paddle::Paddle(
        Modules::Memory::pool_pointer<Modules::Render::Sprite>&&       sprite,
        Modules::Memory::pool_pointer<Modules::Physics::RigidBody2D>&& body)
        : BreakOutEntity(std::move(sprite), std::move(body))
    {
    }

    void Paddle::Position(const Modules::Vector2<float>& pos)
    {
        Sprite().Origin(pos);
        Body().Position(pos);
    }

} // namespace nate::BreakOut
