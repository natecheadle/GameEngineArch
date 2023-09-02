#include "PhysicsSystem.h"

#include "HitShape.h"

#include <3D/Mesh3D.h>
#include <LinearAlgebra/Vector2.hpp>

#include <array>

using namespace std::chrono_literals;

namespace Ignosi::Modules::Physics
{
    PhysicsSystem::PhysicsSystem(ECS::ComponentPool<RigidBody2D>* pRigidBodyPool, Render::Renderer* pRenderer)
        : ECS::System<RigidBody2D>(pRigidBodyPool)
        , m_pRenderer(pRenderer)
    {
    }

    void PhysicsSystem::Update(double dt)
    { /*
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
         }*/
    }

    bool PhysicsSystem::CheckCollision(const RigidBody2D& one, const RigidBody2D& two)
    {
        HitShape* pHitShape1 = one.HitShape().get();
        HitShape* pHitShape2 = two.HitShape().get();

        if ((pHitShape2->Origin() - pHitShape1->Origin()).magnitude() > (pHitShape1->Radius() + pHitShape2->Radius()))
            return false;

        auto evalute_test_axes = [&](const std::vector<Vector2<float>>& axes) -> bool {
            for (const auto& axis : axes)
            {
                std::array<Vector2<float>, 2> hitBox1Projection = pHitShape1->ProjectShape(axis);
                std::array<Vector2<float>, 2> hitBox2Projection = pHitShape2->ProjectShape(axis);

                std::array<std::pair<HitShape*, Vector2<float>>, 4> points = {
                    {{pHitShape1, hitBox1Projection[0]},
                     {pHitShape1, hitBox1Projection[1]},
                     {pHitShape2, hitBox2Projection[0]},
                     {pHitShape2, hitBox2Projection[1]}}
                };

                if (axis.x() > axis.y())
                {
                    std::sort(
                        points.begin(),
                        points.end(),
                        [](const std::pair<HitShape*, Vector2<float>>& lhs,
                           const std::pair<HitShape*, Vector2<float>>& rhs) {
                            return lhs.second.x() > rhs.second.x();
                        });
                }
                else
                {
                    std::sort(
                        points.begin(),
                        points.end(),
                        [](const std::pair<HitShape*, Vector2<float>>& lhs,
                           const std::pair<HitShape*, Vector2<float>>& rhs) {
                            return lhs.second.y() > rhs.second.y();
                        });
                }

                if (points[0].first == points[1].first)
                    return false;
            }
            return true;
        };

        return evalute_test_axes(pHitShape1->TestAxes()) && evalute_test_axes(pHitShape2->TestAxes());
    };

} // namespace Ignosi::Modules::Physics
