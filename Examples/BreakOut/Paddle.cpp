#include "Paddle.h"

#include "BreakOutEntity.h"

#include <IWindow.h>
#include <PhysicsSystem.h>
#include <Renderer/Renderer.h>
#include <World.h>

namespace Ignosi::BreakOut
{
    Paddle::Paddle(BreakOutEntityPointer&& entity, Modules::Render::Renderer* pRenderer, float aspectRatio)
        : CustomBreakOutEntity(std::move(entity))
    {
        World()->AddComponent<Modules::Physics::KinematicData>(Entity());
        World()->AddComponent<Modules::Render::Sprite>(
            Entity(),
            Modules::Render::Sprite(pRenderer, GetComponent<Modules::Physics::KinematicData>(), aspectRatio));
        World()->AddComponent<Modules::Physics::RigidBody2D>(Entity(), GetComponent<Modules::Physics::KinematicData>());
    }

    void Paddle::OnUpdate(double dt)
    {
        auto*                   pWin = World()->Window();
        Modules::Vector3<float> pos  = KinematicData()->Position();
        float                   vel  = static_cast<float>(m_PaddleMoveSpeed * dt);

        pWin->ExecuteWithKeyStates([&](const Modules::GUI::KeyStateMap& keyStates) {
            if ((keyStates[Modules::GUI::Key::Left].first == Modules::GUI::KeyState::Pressed ||
                 keyStates[Modules::GUI::Key::Left].first == Modules::GUI::KeyState::Repeat))
            {
                if (pos[0] > 0.0)
                {
                    pos.x(pos.x() - vel);
                    KinematicData()->Position(pos);
                }
            }

            if ((keyStates[Modules::GUI::Key::Right].first == Modules::GUI::KeyState::Pressed ||
                 keyStates[Modules::GUI::Key::Right].first == Modules::GUI::KeyState::Repeat))
            {
                if (pos[0] < static_cast<float>(pWin->GetLastWindowSize().Width()) - Sprite()->Size()[0])
                {
                    pos.x(pos.x() + vel);
                    KinematicData()->Position(pos);
                }
            }
        });
    }

} // namespace Ignosi::BreakOut
