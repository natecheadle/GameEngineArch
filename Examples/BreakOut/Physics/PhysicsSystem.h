#pragma once

#include "RigidBody2D.h"

#include <System.h>

#include <chrono>
#include <vector>

namespace nate::Modules::Physics
{
    class PhysicsSystem : public ECS::System<RigidBody2D>
    {
        std::vector<RigidBody2D*> m_MovingObjects;

      public:
        PhysicsSystem(Memory::PoolMemoryBlock<RigidBody2D>* pRigidBodyPool);

        void Update(std::chrono::nanoseconds dt);

      private:
        static bool CheckCollision(const RigidBody2D& one, const RigidBody2D& two);
    };
} // namespace nate::Modules::Physics
