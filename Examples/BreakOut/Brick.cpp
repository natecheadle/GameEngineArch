#include "Brick.h"

#include <PhysicsSystem.h>
#include <Renderer/Renderer.h>
#include <World.h>

#include <utility>

using namespace std::placeholders;

namespace Ignosi::BreakOut
{
    Brick::Brick(BreakOutEntityPointer&& entity, Modules::Render::Renderer* pRenderer)
        : CustomBreakOutEntity(std::move(entity))
    {
        World()->AddComponent<Modules::Physics::KinematicData>(Entity());
        World()->AddComponent<Modules::Render::Sprite>(
            Entity(),
            Modules::Render::Sprite(pRenderer, GetComponent<Modules::Physics::KinematicData>()));
        World()->AddComponent<Modules::Physics::RigidBody2D>(Entity(), GetComponent<Modules::Physics::KinematicData>());

        World()->RegisterEntityInSystem(*pRenderer, Entity());
        World()->RegisterEntityInSystem(*(World()->GetSystem<Modules::Physics::PhysicsSystem>()), Entity());

        m_Subscription = Body()->SubscribeOnCollision(std::bind(&Brick::OnCollision, this, _1));
    }

    Brick::~Brick() {}

    Brick::Brick(Brick&& other)
        : CustomBreakOutEntity(std::move(other))
        , m_Type(other.Type())
    {
        m_Subscription = Body()->SubscribeOnCollision(std::bind(&Brick::OnCollision, this, _1));
    }

    Brick& Brick::operator=(Brick&& other)
    {
        CustomBreakOutEntity::operator=(std::move(other));

        m_Type         = other.Type();
        m_Subscription = Body()->SubscribeOnCollision(std::bind(&Brick::OnCollision, this, _1));
        return *this;
    }

    void Brick::OnCollision(const Modules::Physics::RigidBody2D& /* other */)
    {
        // TODO Implement collision
    }
} // namespace Ignosi::BreakOut
