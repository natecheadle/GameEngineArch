#pragma once

#include "BreakOutEntity.h"
#include "IWindow.h"
#include "Renderer/Renderer.h"

namespace Ignosi::BreakOut
{
    class Paddle : public CustomBreakOutEntity
    {
        double m_PaddleMoveSpeed{0.0};

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

        void   PaddleMoveSpeed(double value) { m_PaddleMoveSpeed = value; }
        double PaddleMoveSpeed() const { return m_PaddleMoveSpeed; }

      protected:
        void OnUpdate(double dt) override;
    };
} // namespace Ignosi::BreakOut
