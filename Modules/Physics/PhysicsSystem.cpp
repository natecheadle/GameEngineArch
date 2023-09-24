#include "PhysicsSystem.h"

#include "ComponentPointer.h"
#include "HitShape.h"
#include "IEntity.h"
#include "KinematicData.h"
#include "LinearAlgebra/Vector3.hpp"
#include "RigidBody2D.h"
#include "Tag.h"

#include <LinearAlgebra/Vector2.hpp>

#include <array>

using namespace std::chrono_literals;

namespace Ignosi::Modules::Physics
{
    PhysicsSystem::PhysicsSystem(ECS::ComponentPool<RigidBody2D>* pRigidBodyPool, ECS::ComponentPool<KinematicData>* pKinematicDataPool)
        : ECS::System<RigidBody2D, KinematicData>(pRigidBodyPool, pKinematicDataPool)
    {
    }

    void PhysicsSystem::Update(double dt)
    {
        float                      del_t             = static_cast<float>(dt);
        auto&                      rigidBodyPool     = GetPool<RigidBody2D>();
        auto&                      kinematicDataPool = GetPool<KinematicData>();
        std::vector<ECS::EntityID> movingEntities;
        const auto&                kinematicEntities = World()->GetEntitiesByTag(ECS::ComponentPool<KinematicData>::ComponentTag());
        for (const auto& entity : kinematicEntities)
        {
            auto& kinematicData = kinematicDataPool.GetComponent(World()->GetEntity(entity));
            kinematicData.Update(dt);
            if (kinematicData.Velocity() != Vector3<float>(0.0f, 0.0f, 0.0f))
            {
                movingEntities.push_back(entity);
            }
        }

        const auto& rigidBodyEntities = World()->GetEntitiesByTag(ECS::ComponentPool<RigidBody2D>::ComponentTag());

        for (const auto& entity1 : movingEntities)
        {
            for (const auto& entity2 : rigidBodyEntities)
            {
                if (entity1 == entity2)
                    continue;

                if (rigidBodyPool.HasComponent(World()->GetEntity(entity1)) && rigidBodyPool.HasComponent(World()->GetEntity(entity2)))
                {
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
    }

    bool PhysicsSystem::CheckCollision(const RigidBody2D& one, const RigidBody2D& two)
    {
        HitShape* pHitShape1 = one.HitShape().get();
        HitShape* pHitShape2 = two.HitShape().get();

        if (!pHitShape1 || !pHitShape2)
            return false;

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
