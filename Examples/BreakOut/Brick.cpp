#include "Brick.h"

#include <HitRectangle.h>
#include <PhysicsSystem.h>
#include <Renderer/Renderer.h>
#include <World.h>

#include <utility>

using namespace std::placeholders;

namespace Ignosi::BreakOut
{
    Brick::Brick(BreakOutWorld& world, Modules::Vector2<float> size, Modules::Vector2<float> position, BrickType type)
        : CustomBreakOutEntity(world.CreateEntity())
        , m_Type(type)
    {
        assert(m_Type != BrickType::None);

        Modules::Render::Renderer* pRenderer = World()->GetSystem<Modules::Render::Renderer>();
        World()->AddComponent<Modules::Physics::KinematicData>(Entity());
        World()->AddComponent<Modules::Render::Sprite>(
            Entity(),
            Modules::Render::Sprite(pRenderer, GetComponent<Modules::Physics::KinematicData>()));
        World()->AddComponent<Modules::Physics::RigidBody2D>(Entity(), GetComponent<Modules::Physics::KinematicData>());

        m_Subscription = Body()->SubscribeOnCollision(std::bind(&Brick::OnCollision, this, _1));

        Sprite()->Size(size);
        Body()->HitShape(std::make_unique<Modules::Physics::HitRectangle>(KinematicData(), size.x(), size.y()));
        KinematicData()->Position(position);

        switch (m_Type)
        {
        case BrickType::Solid: Sprite()->Color({0.8f, 0.8f, 0.7f}); break;
        case BrickType::BabyBlue: Sprite()->Color({0.2f, 0.6f, 1.0f}); break;
        case BrickType::Green: Sprite()->Color({0.0f, 0.7f, 0.0f}); break;
        case BrickType::LightBlue: Sprite()->Color({0.8f, 0.8f, 0.4f}); break;
        case BrickType::Orange: Sprite()->Color({1.0f, 0.5f, 0.0f}); break;
        default: break;
        }
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
