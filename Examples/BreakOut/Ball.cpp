#include "Ball.h"

#include "3D/Model3D.h"
#include "BreakOutEntity.h"
#include "RigidBody2D.h"

#include <HitCircle.h>
#include <PhysicsSystem.h>
#include <Renderer/Renderer.h>
#include <World.h>

#include <memory>

using namespace std::chrono_literals;
using namespace std::placeholders;

namespace Ignosi::BreakOut
{
    Ball::Ball(BreakOutEntityPointer&& entity, Modules::Render::Renderer* pRenderer)
        : CustomBreakOutEntity(std::move(entity))
    {
        World()->AddComponent<Modules::Physics::KinematicData>(Entity());
        World()->AddComponent<Modules::Render::Sprite>(
            Entity(),
            Modules::Render::Sprite(pRenderer, GetComponent<Modules::Physics::KinematicData>()));
        World()->AddComponent<Modules::Physics::RigidBody2D>(Entity(), GetComponent<Modules::Physics::KinematicData>());

        Sprite()->SizeX(m_Radius * 2);
        Sprite()->SizeY(m_Radius * 2);
        Body()->HitShape(std::make_unique<Modules::Physics::HitCircle>(KinematicData(), m_Radius));
        m_Subscription = Body()->SubscribeOnCollision(std::bind(&Ball::OnCollision, this, _1));
    }

    Ball::~Ball() {}

    Ball::Ball(Ball&& other)
        : CustomBreakOutEntity(std::move(other))
        , m_Radius(other.m_Radius)
        , m_WinWidth(other.m_WinWidth)
        , m_InitialVel(std::move(other.m_InitialVel))
    {
        m_Subscription = Body()->SubscribeOnCollision(std::bind(&Ball::OnCollision, this, _1));
    }

    Ball& Ball::operator=(Ball&& other)
    {
        CustomBreakOutEntity::operator=(std::move(other));

        m_Radius     = other.m_Radius;
        m_WinWidth   = other.m_WinWidth;
        m_InitialVel = std::move(other.m_InitialVel);

        m_Subscription = Body()->SubscribeOnCollision(std::bind(&Ball::OnCollision, this, _1));

        return *this;
    }

    void Ball::OnCollision(const Modules::Physics::RigidBody2D& other)
    {
        // TODO - Need To Implement
    }
} // namespace Ignosi::BreakOut
