#pragma once

#include "3D/Mesh3D.h"
#include "RigidBody2D.h"

#include <System.h>

#include <chrono>
#include <vector>

namespace Ignosi::Modules::Physics
{
    class PhysicsSystem : public ECS::System<RigidBody2D>
    {
        std::vector<RigidBody2D*> m_MovingObjects;
        Render::Renderer*         m_pRenderer;

      public:
        PhysicsSystem(Memory::PoolMemoryBlock<RigidBody2D>* pRigidBodyPool, Render::Renderer* pRenderer = nullptr);

        void Update(std::chrono::nanoseconds dt);

      private:
        static bool CheckCollision(const RigidBody2D& one, const RigidBody2D& two);
    };
} // namespace Ignosi::Modules::Physics
