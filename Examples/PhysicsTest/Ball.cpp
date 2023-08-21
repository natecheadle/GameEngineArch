#include "Ball.h"

#include "HitCircle.h"

using namespace std::placeholders;

namespace Ignosi::Examples::PhysicsTest
{
    Ball::Ball(
        Modules::Memory::pool_pointer<Modules::Render::Sprite>       sprite,
        Modules::Memory::pool_pointer<Modules::Physics::RigidBody2D> rigidBody)
        : BASE(std::move(sprite), (std::move(rigidBody)))
    {
        RigidBody().AttachOnPosChange(std::bind(&Ball::OnPositionUpdate, this, _1));
        RigidBody().AttachOnCollision(std::bind(&Ball::OnCollision, this, _1));
    }

    Ball::~Ball()
    {
        auto* pBody = GetPointer<Modules::Physics::RigidBody2D>();
        if (pBody)
        {
            pBody->ClearCallbacks();
        }
    }

    Ball::Ball(Ball&& other)
        : BASE(std::move(other))
    {
        RigidBody().AttachOnPosChange(std::bind(&Ball::OnPositionUpdate, this, _1));
        RigidBody().AttachOnCollision(std::bind(&Ball::OnCollision, this, _1));
    }

    Ball& Ball::operator=(Ball&& other)
    {
        BASE::operator=(std::move(other));

        RigidBody().AttachOnPosChange(std::bind(&Ball::OnPositionUpdate, this, _1));

        return *this;
    }

    void Ball::OnPositionUpdate(const Modules::Vector2<float>& value)
    {
        Sprite().Origin(value);
    }

    void Ball::OnCollision(const Modules::Physics::RigidBody2D& other)
    {
        auto  vel    = RigidBody().Velocity();
        float radius = RigidBody().HitShape()->Radius();
        if (std::abs(other.Position().x() - RigidBody().Position().x()) < radius)
        {
            vel.x(RigidBody().Velocity().x() * -1);
        }
        else
        {
            vel.y(RigidBody().Velocity().y() * -1);
        }
        RigidBody().Velocity(vel);
    }
} // namespace Ignosi::Examples::PhysicsTest
