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
        PhysicsSystem(ECS::ComponentPool<RigidBody2D>* pRigidBodyPool, Render::Renderer* pRenderer = nullptr);
        std::string_view Name() const override { return "Physics System"; }
        std::uint32_t    Priority() const override { return 0; }
        void             Update(double dt) override;

      private:
        static bool CheckCollision(const RigidBody2D& one, const RigidBody2D& two);
    };
} // namespace Ignosi::Modules::Physics
