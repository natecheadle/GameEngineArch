#pragma once

#include "BreakOutEntity.h"

#include <LinearAlgebra/Vector2.hpp>

#include <chrono>

namespace nate::BreakOut
{
    class Ball : public BreakOutEntity
    {
        float                   m_Radius{12.5};
        float                   m_WinWidth{600.0};
        Modules::Vector2<float> m_InitialVel{.1f, -1.0f};

      public:
        Ball(
            Modules::Memory::pool_pointer<Modules::Render::Sprite>&&       sprite,
            Modules::Memory::pool_pointer<Modules::Physics::RigidBody2D>&& body);

        ~Ball();

        Ball(Ball&& other);
        Ball& operator=(Ball&& other);

        void                           Position(const Modules::Vector2<float>& pos);
        const Modules::Vector2<float>& Position() const { return Body().Position(); }

        void  WindowWidth(float val) { m_WinWidth = val; }
        float WindowWidth() const { return m_WinWidth; }

        void  Radius(float val) { m_Radius = val; }
        float Radius() const { return m_Radius; }

        void Release() { Body().Velocity(m_InitialVel); }
        bool IsStuck() const { return Body().Velocity() == Modules::Vector2<float>{0.0, 0.0}; }

      private:
        Modules::Render::Sprite&       Sprite() { return BreakOutEntity::Get<Modules::Render::Sprite>(); }
        const Modules::Render::Sprite& Sprite() const { return BreakOutEntity::Get<Modules::Render::Sprite>(); }

        Modules::Physics::RigidBody2D&       Body() { return BreakOutEntity::Get<Modules::Physics::RigidBody2D>(); }
        const Modules::Physics::RigidBody2D& Body() const
        {
            return BreakOutEntity::Get<Modules::Physics::RigidBody2D>();
        }

        void OnCollision(const Modules::Physics::RigidBody2D& other);
        void OnPosChange(const Modules::Vector2<float>& newPos) { Sprite().Origin(newPos); }
    };
} // namespace nate::BreakOut
