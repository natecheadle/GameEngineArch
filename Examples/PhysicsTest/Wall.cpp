#include "Wall.h"

namespace Ignosi::Examples::PhysicsTest
{
    Wall::Wall(
        Modules::Memory::pool_pointer<Modules::Render::Sprite>       sprite,
        Modules::Memory::pool_pointer<Modules::Physics::RigidBody2D> rigidBody)
        : BASE(std::move(sprite), (std::move(rigidBody)))
    {
    }
} // namespace Ignosi::Examples::PhysicsTest
