#include "PhysicsSystem.h"

#include <LinearAlgebra/Vector2.hpp>

using namespace std::chrono_literals;

namespace Ignosi::Modules::Physics
{
    PhysicsSystem::PhysicsSystem(Memory::PoolMemoryBlock<RigidBody2D>* pRigidBodyPool)
        : ECS::System<RigidBody2D>(pRigidBodyPool)
    {
    }

    void PhysicsSystem::Update(std::chrono::nanoseconds dt)
    {
        float del_t = static_cast<float>(dt / 1.0s);
        auto& pool  = GetPool<RigidBody2D>();
        m_MovingObjects.clear();
        for (auto& body : pool)
        {
            body.Update(del_t);
            if (body.Velocity() != Vector2<float>{0.0f, 0.0f})
            {
                m_MovingObjects.push_back(&body);
                Vector2<float> pos = body.Position();
                Vector2<float> vel = body.Velocity();
                pos.x(pos.x() + vel.x());
                pos.y(pos.y() + vel.y());

                body.Position(pos);
            }
        }

        for (auto& pBody : m_MovingObjects)
        {
            for (auto& body : pool)
            {
                if (&body == pBody)
                    continue;

                if (CheckCollision(*pBody, body))
                {
                    pBody->CollisionOccurred(body);
                    body.CollisionOccurred(*pBody);
                }
            }
        }
    }

    bool PhysicsSystem::CheckCollision(const RigidBody2D& one, const RigidBody2D& two)
    {
        return false;
    }
} // namespace Ignosi::Modules::Physics
