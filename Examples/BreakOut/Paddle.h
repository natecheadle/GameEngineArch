#pragma once

#include "BreakOutEntity.h"
#include "Renderer/Renderer.h"

namespace Ignosi::BreakOut
{
    class Paddle : public CustomBreakOutEntity
    {
      public:
        Paddle(BreakOutEntityPointer&& entity, Modules::Render::Renderer* pRenderer, float aspectRatio);

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
    };
} // namespace Ignosi::BreakOut
