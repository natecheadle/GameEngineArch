#include "Brick.h"

#include "BreakOutEntity.h"
#include "Physics/RigidBody2D.h"

#include <utility>

namespace nate::BreakOut
{
    Brick::Brick(
        Modules::Memory::pool_pointer<Modules::Render::Sprite>&&       sprite,
        Modules::Memory::pool_pointer<Modules::Physics::RigidBody2D>&& body)
        : BreakOutEntity(std::move(sprite), std::move(body))
    {
        using namespace std::placeholders;
        Body().AttachOnCollision(std::bind(&Brick::OnCollision, this, _1));
    }

    Brick::~Brick()
    {
        Body().ClearCallbacks();
    }

    void Brick::Position(const Modules::Vector2<float>& pos)
    {
        Sprite().Origin(pos);
        Body().Position(pos);
    }

    void Brick::OnCollision(const Modules::Physics::RigidBody2D& /* other */)
    {
        Sprite().Color({0.0, 0.0, 0.0});
    }
} // namespace nate::BreakOut
