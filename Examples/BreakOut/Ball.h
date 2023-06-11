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
        bool                    m_IsStuck;
        Modules::Vector2<float> m_Velocity{1.5f, -10.0f};

      public:
        Ball(
            Modules::Memory::pool_pointer<Modules::Render::Sprite>&&       sprite,
            Modules::Memory::pool_pointer<Modules::Physics::RigidBody2D>&& body);

        void                           Position(const Modules::Vector2<float>& pos);
        const Modules::Vector2<float>& Position() const { return Body().Position(); }

        void  WindowWidth(float val) { m_WinWidth = val; }
        float WindowWidth() const { return m_WinWidth; }

        void  Radius(float val) { m_Radius = val; }
        float Radius() const { return m_Radius; }

        void Update(std::chrono::nanoseconds del);

        void Release() { m_IsStuck = false; }
        bool IsStuck() const { return m_IsStuck; }

      private:
        Modules::Render::Sprite&       Sprite() { return BreakOutEntity::Get<Modules::Render::Sprite>(); }
        const Modules::Render::Sprite& Sprite() const { return BreakOutEntity::Get<Modules::Render::Sprite>(); }

        Modules::Physics::RigidBody2D&       Body() { return BreakOutEntity::Get<Modules::Physics::RigidBody2D>(); }
        const Modules::Physics::RigidBody2D& Body() const
        {
            return BreakOutEntity::Get<Modules::Physics::RigidBody2D>();
        }
    };
} // namespace nate::BreakOut
