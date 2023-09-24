#include "Wall.h"

#include "BreakOutEntity.h"

#include <HitRectangle.h>
#include <PhysicsSystem.h>
#include <World.h>

namespace Ignosi::BreakOut
{
    Wall::Wall(BreakOutWorld& world, Modules::Vector2<float> size, Modules::Vector2<float> position)
        : CustomBreakOutEntity(world.CreateEntity())
    {
        World()->AddComponent<Modules::Physics::KinematicData>(Entity());
        World()->AddComponent<Modules::Physics::RigidBody2D>(Entity(), GetComponent<Modules::Physics::KinematicData>());

        Body()->HitShape(std::make_unique<Modules::Physics::HitRectangle>(KinematicData(), size.x(), size.y()));
        KinematicData()->Position(position);
    }

} // namespace Ignosi::BreakOut
