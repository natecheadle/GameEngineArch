#pragma once

#include "BreakOutEntity.h"

#include <LinearAlgebra/Vector3.hpp>

#include <chrono>
#include <memory>

namespace Ignosi::BreakOut
{
    class Ball : public CustomBreakOutEntity
    {
        float                   m_Radius{12.5};
        float                   m_WinWidth{600.0};
        Modules::Vector3<float> m_InitialVel{.1f, -1.0f, 0.0};

        std::unique_ptr<Modules::Messaging::EventSubscriber<const Modules::Physics::RigidBody2D&>> m_Subscription;

      public:
        Ball(BreakOutEntityPointer&& entity, Modules::Render::Renderer* pRenderer);
        ~Ball();

        Ball(Ball&& other);
        Ball& operator=(Ball&& other);

        Modules::ECS::WeakComponentPointer<Modules::Physics::KinematicData> KinematicData() const
        {
            return GetComponent<Modules::Physics::KinematicData>();
        }

        Modules::ECS::WeakComponentPointer<Modules::Render::Sprite> Sprite() const { return GetComponent<Modules::Render::Sprite>(); }

        Modules::ECS::WeakComponentPointer<Modules::Physics::RigidBody2D> Body() const
        {
            return GetComponent<Modules::Physics::RigidBody2D>();
        }

        void  WindowWidth(float val) { m_WinWidth = val; }
        float WindowWidth() const { return m_WinWidth; }

        void  Radius(float val) { m_Radius = val; }
        float Radius() const { return m_Radius; }

        void Release() { KinematicData()->Velocity(m_InitialVel); }
        bool IsStuck() const { return KinematicData()->Velocity() == Modules::Vector3<float>{0.0, 0.0, 0.0}; }

      protected:
        void OnUpdate(double dt) override {}

      private:
        void OnCollision(const Modules::Physics::RigidBody2D& other);
    };
} // namespace Ignosi::BreakOut
