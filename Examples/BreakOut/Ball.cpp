#include "Ball.h"

using namespace std::chrono_literals;

namespace nate::BreakOut
{
    Ball::Ball(
        Modules::Memory::pool_pointer<Modules::Render::Sprite>&&       sprite,
        Modules::Memory::pool_pointer<Modules::Physics::RigidBody2D>&& body)
        : BreakOutEntity(std::move(sprite), std::move(body))
        , m_IsStuck(true)
    {
        Sprite().SizeX(m_Radius * 2);
        Sprite().SizeY(m_Radius * 2);
    }

    void Ball::Position(const Modules::Vector2<float>& pos)
    {
        Sprite().Origin(pos);
        Body().Position(pos);
    }

    void Ball::Update(std::chrono::nanoseconds /* del */)
    {
        if (!m_IsStuck)
        {
            Modules::Vector2<float> pos  = Sprite().Origin();
            Modules::Vector2<float> size = Sprite().Size();

            pos += m_Velocity;

            if (pos[0] <= 0.0f)
            {
                m_Velocity[0] = -m_Velocity[0];
                pos[0]        = 0.0f;
            }
            else if (pos[0] + size[0] >= m_WinWidth)
            {
                m_Velocity[0] = -m_Velocity[0];
                pos[0]        = m_WinWidth - size[0];
            }
            if (pos[1] <= 0.0f)
            {
                m_Velocity[1] = -m_Velocity[1];
                pos[1]        = 0.0f;
            }

            Sprite().Origin(pos);
            Body().Position(pos);
        }
    }
} // namespace nate::BreakOut
