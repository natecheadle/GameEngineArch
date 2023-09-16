#include "Wall.h"

#include "BreakOutEntity.h"

#include <PhysicsSystem.h>
#include <World.h>

namespace Ignosi::BreakOut
{
    Wall::Wall(BreakOutEntityPointer&& entity)
        : CustomBreakOutEntity(std::move(entity))
    {
        World()->AddComponent<Modules::Physics::KinematicData>(Entity());
        World()->AddComponent<Modules::Physics::RigidBody2D>(Entity(), GetComponent<Modules::Physics::KinematicData>());

        World()->RegisterEntityInSystem(*(World()->GetSystem<Modules::Physics::PhysicsSystem>()), Entity());
    }

} // namespace Ignosi::BreakOut
