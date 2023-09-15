#include "Paddle.h"

#include "BreakOutEntity.h"

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

} // namespace Ignosi::BreakOut
