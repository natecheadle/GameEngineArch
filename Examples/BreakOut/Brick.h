#pragma once

#include "BreakOutEntity.h"
#include "Renderer/Renderer.h"

#include <utility>

namespace Ignosi::BreakOut
{
    enum class BrickType
    {
        None     = 0,
        Solid    = 1,
        BabyBlue = 2,
        Green    = 3,
    };

    class Brick : public CustomBreakOutEntity
    {
        BrickType                                                                                  m_Type;
        std::unique_ptr<Modules::Messaging::EventSubscriber<const Modules::Physics::RigidBody2D&>> m_Subscription;

      public:
        Brick(BreakOutEntityPointer&& entity, Modules::Render::Renderer* pRenderer);
        ~Brick();

        Brick(Brick&& other);
        Brick& operator=(Brick&& other);

        BrickType Type() const { return m_Type; }
        void      Type(BrickType val) { m_Type = val; }

        Modules::ECS::WeakComponentPointer<Modules::Physics::KinematicData> KinematicData() const
        {
            return GetComponent<Modules::Physics::KinematicData>();
        }

        Modules::ECS::WeakComponentPointer<Modules::Render::Sprite> Sprite() const { return GetComponent<Modules::Render::Sprite>(); }

        Modules::ECS::WeakComponentPointer<Modules::Physics::RigidBody2D> Body() const
        {
            return GetComponent<Modules::Physics::RigidBody2D>();
        }

      protected:
        void OnUpdate(double dt) override {}

      private:
        void OnCollision(const Modules::Physics::RigidBody2D& other);
    };
} // namespace Ignosi::BreakOut
