#include "Ball.h"

#include "BreakOutEntity.h"

using namespace std::chrono_literals;
using namespace std::placeholders;

namespace nate::BreakOut
{
    Ball::Ball(
        Modules::Memory::pool_pointer<Modules::Render::Sprite>&&       sprite,
        Modules::Memory::pool_pointer<Modules::Physics::RigidBody2D>&& body)
        : BreakOutEntity(std::move(sprite), std::move(body))
    {
        Sprite().SizeX(m_Radius * 2);
        Sprite().SizeY(m_Radius * 2);
        Body().HitBox({m_Radius * 2.0f, m_Radius * 2.0f});

        Body().AttachOnCollision(std::bind(&Ball::OnCollision, this, _1));
        Body().AttachOnPosChange(std::bind(&Ball::OnPosChange, this, _1));
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
        : BreakOutEntity(std::move(other))
        , m_Radius(other.m_Radius)
        , m_WinWidth(other.m_WinWidth)
        , m_InitialVel(std::move(other.m_InitialVel))
    {
        Body().AttachOnCollision(std::bind(&Ball::OnCollision, this, _1));
        Body().AttachOnPosChange(std::bind(&Ball::OnPosChange, this, _1));
    }
    Ball& Ball::operator=(Ball&& other)
    {
        BreakOutEntity::operator=(std::move(other));

        m_Radius     = other.m_Radius;
        m_WinWidth   = other.m_WinWidth;
        m_InitialVel = std::move(other.m_InitialVel);

        Body().AttachOnCollision(std::bind(&Ball::OnCollision, this, _1));
        Body().AttachOnPosChange(std::bind(&Ball::OnPosChange, this, _1));

        return *this;
    }

    void Ball::Position(const Modules::Vector2<float>& pos)
    {
        Sprite().Origin(pos);
        Body().Position(pos);
    }

    void Ball::OnCollision(const Modules::Physics::RigidBody2D& other)
    {
        auto vel = Body().Velocity();
        if (std::abs(other.Position().x() - Body().Position().x()) < m_Radius)
        {
            vel.x(Body().Velocity().x() * -1);
        }
        else
        {
            vel.y(Body().Velocity().y() * -1);
        }
        Body().Velocity(vel);
    }
} // namespace nate::BreakOut
