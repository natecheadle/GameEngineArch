#include "Brick.h"

#include "BreakOutEntity.h"
#include "Physics/RigidBody2D.h"

#include <utility>

using namespace std::placeholders;

namespace nate::BreakOut
{
    Brick::Brick(
        Modules::Memory::pool_pointer<Modules::Render::Sprite>&&       sprite,
        Modules::Memory::pool_pointer<Modules::Physics::RigidBody2D>&& body)
        : BreakOutEntity(std::move(sprite), std::move(body))
    {
        Body().AttachOnCollision(std::bind(&Brick::OnCollision, this, _1));
    }

    Brick::~Brick()
    {
        auto* pBody = GetPointer<Modules::Physics::RigidBody2D>();
        if (pBody)
        {
            pBody->ClearCallbacks();
        }
    }

    Brick::Brick(Brick&& other)
        : BreakOutEntity(std::move(other))
        , m_Type(other.Type())
    {
        Body().AttachOnCollision(std::bind(&Brick::OnCollision, this, _1));
    }

    Brick& Brick::operator=(Brick&& other)
    {
        BreakOutEntity::operator=(std::move(other));

        m_Type = other.Type();
        Body().AttachOnCollision(std::bind(&Brick::OnCollision, this, _1));
        return *this;
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
