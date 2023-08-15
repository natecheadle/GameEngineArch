#include "PhysicsSystem.h"

#include <3D/Mesh3D.h>
#include <LinearAlgebra/Vector2.hpp>

#include <array>

using namespace std::chrono_literals;

namespace Ignosi::Modules::Physics
{
    PhysicsSystem::PhysicsSystem(Memory::PoolMemoryBlock<RigidBody2D>* pRigidBodyPool, Render::Renderer* pRenderer)
        : ECS::System<RigidBody2D>(pRigidBodyPool)
        , m_pRenderer(pRenderer)
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

        std::vector<float> projectedPoints;

        for (size_t i = 0; i < one.HitBoxes().size(); ++i)
        {
            // TODO - This test twice for some hitboxes need to skip duplicates.
            for (size_t j = 0; j < two.HitBoxes().size(); ++j)
            {
                const auto& test_axes = one.HitBoxes()[i]->TestAxes(*two.HitBoxes()[j]);
                bool        collision = true;

                for (const auto& axis : test_axes)
                {
                    // If we detect any gaps abort.
                    if (!collision)
                        break; // for(axis : test_axes)

                    std::array<Vector2<float>, 2> hitBox1Projection = one.HitBoxes()[i]->ProjectShape(axis);
                    std::array<Vector2<float>, 2> hitBox2Projection = two.HitBoxes()[j]->ProjectShape(axis);

                    std::array<const Vector2<float>*, 4> allProjectsion = {
                        {&hitBox1Projection[0], &hitBox1Projection[1], &hitBox2Projection[0], &hitBox2Projection[1]}
                    };

                    if (axis.x() > axis.y())
                    {
                        std::sort(
                            allProjectsion.begin(),
                            allProjectsion.end(),
                            [](const Vector2<float>* lhs, const Vector2<float>* rhs) { return lhs->x() > rhs->x(); });
                    }
                    else
                    {
                        std::sort(
                            allProjectsion.begin(),
                            allProjectsion.end(),
                            [](const Vector2<float>* lhs, const Vector2<float>* rhs) { return lhs->y() > rhs->y(); });
                    }

                    if ((&hitBox1Projection[0] == allProjectsion[0] && &hitBox1Projection[1] == allProjectsion[1]) ||
                        (&hitBox1Projection[1] == allProjectsion[0] && &hitBox1Projection[0] == allProjectsion[1]) ||
                        (&hitBox2Projection[0] == allProjectsion[0] && &hitBox2Projection[1] == allProjectsion[1]) ||
                        (&hitBox2Projection[1] == allProjectsion[0] && &hitBox2Projection[0] == allProjectsion[1]))
                    {
                        collision = false;
                    }
                }

                if (collision)
                    return true;
            }
        }

        return false;
    }
} // namespace Ignosi::Modules::Physics
