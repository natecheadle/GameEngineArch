#include "PhysicsSystem.h"

#include "HitShape.h"
#include "KinematicData.h"
#include "Tag.h"

#include <LinearAlgebra/Vector2.hpp>

#include <array>

using namespace std::chrono_literals;

namespace Ignosi::Modules::Physics
{
    PhysicsSystem::PhysicsSystem(ECS::ComponentPool<RigidBody2D>* pRigidBodyPool, ECS::ComponentPool<KinematicData>* pKinematicDataPool)
        : ECS::System<RigidBody2D, KinematicData>(pRigidBodyPool, pKinematicDataPool)
        , m_SystemTag(ECS::Tag::Create(PhysicsSystem::NAME))
    {
    }

    void PhysicsSystem::Update(double dt)
    {
        float del_t             = static_cast<float>(dt);
        auto& rigidBodyPool     = GetPool<RigidBody2D>();
        auto& kinematicDataPool = GetPool<KinematicData>();

        const auto& entities = World()->GetEntitiesByTag(Tag());
        for (const auto& entity : entities)
        {
            auto& kinematicData = kinematicDataPool.GetComponent(World()->GetEntity(entity));
            kinematicData.Update(dt);
        }

        for (const auto& entity1 : entities)
        {
            for (const auto& entity2 : entities)
            {
                if (entity1 == entity2)
                    continue;

                auto& body1 = rigidBodyPool.GetComponent(World()->GetEntity(entity1));
                auto& body2 = rigidBodyPool.GetComponent(World()->GetEntity(entity2));

                if (CheckCollision(body1, body2))
                {
                    body1.CollisionOccurred(body2);
                    body2.CollisionOccurred(body1);
                }
            }
        }
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
                        [](const std::pair<HitShape*, Vector2<float>>& lhs, const std::pair<HitShape*, Vector2<float>>& rhs) {
                            return lhs.second.x() > rhs.second.x();
                        });
                }
                else
                {
                    std::sort(
                        points.begin(),
                        points.end(),
                        [](const std::pair<HitShape*, Vector2<float>>& lhs, const std::pair<HitShape*, Vector2<float>>& rhs) {
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
