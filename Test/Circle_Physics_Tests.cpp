#include <HitCircle.h>
#include <HitRectangle.h>
#include <HitShape.h>
#include <LinearAlgebra/Vector2.hpp>
#include <PhysicsSystem.h>
#include <PoolMemoryBlock.hpp>
#include <RigidBody2D.h>
#include <Units/Radian.hpp>
#include <World.h>

#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <memory>
#include <ostream>
#include <utility>
#include <vector>
using namespace Ignosi::Modules;

namespace Ignosi::Test
{
    struct CircleCollisionInputData
    {
        CircleCollisionInputData(
            const Vector2<float>& origin1,
            float                 rotation1,
            const Vector2<float>& origin2,
            bool                  result)
        {
            Origin1   = origin1;
            Origin2   = origin2;
            Rotation1 = rotation1;
            Result    = result;
        }

        Vector2<float> Origin1;
        float          Rotation1;
        Vector2<float> Origin2;
        bool           Result;
    };

    namespace
    {
        std::vector<CircleCollisionInputData> GetCircleTests()
        {
            std::vector<CircleCollisionInputData> tests;

            tests.push_back(CircleCollisionInputData({0.0f, 0.0f}, 0.0f, {1.0f, 1.0f}, true));
            tests.push_back(CircleCollisionInputData({0.0f, 0.0f}, 0.0f, {0.5f, 0.5f}, true));
            tests.push_back(CircleCollisionInputData({0.0f, 0.0f}, 0.0f, {3.0f, 3.0f}, false));
            tests.push_back(CircleCollisionInputData({0.0f, 0.0f}, 0.0f, {0.5f, 0.5f}, true));
            tests.push_back(CircleCollisionInputData({0.5f, 0.5f}, -M_PI_4, {0.0f, 0.0f}, false));

            return tests;
        }
    } // namespace

    class Circle_Physics_Tests : public testing::TestWithParam<CircleCollisionInputData>
    {
      protected:
        ECS::World<Physics::RigidBody2D>                      m_World;
        Physics::PhysicsSystem*                               m_PhysicsSystem;
        std::vector<ECS::EntityPointer<Physics::RigidBody2D>> m_Entities;

      public:
        Circle_Physics_Tests()
            : m_PhysicsSystem(m_World.CreateSystem<Physics::PhysicsSystem, Physics::RigidBody2D>())
        {
            m_Entities.push_back(m_World.CreateEntity<Physics::RigidBody2D>());
            m_Entities.push_back(m_World.CreateEntity<Physics::RigidBody2D>());

            m_World.RegisterEntityInSystem(*m_PhysicsSystem, m_Entities[0]);
            m_World.RegisterEntityInSystem(*m_PhysicsSystem, m_Entities[1]);
        }
    };

    TEST_P(Circle_Physics_Tests, ValidateCircleCollision)
    {
        auto test = GetParam();

        bool collisionOccurred = false;
        auto onCollision       = [&](const Physics::RigidBody2D&) { collisionOccurred = true; };

        auto pRigidBody1 = m_Entities[0]->Get<Physics::RigidBody2D>();
        auto pRigidBody2 = m_Entities[1]->Get<Physics::RigidBody2D>();

        pRigidBody2->AttachOnCollision(onCollision);
        pRigidBody1->AttachOnCollision(onCollision);

        std::unique_ptr<Physics::HitRectangle> pRect1  = std::make_unique<Physics::HitRectangle>(nullptr);
        std::unique_ptr<Physics::HitCircle>    pCircle = std::make_unique<Physics::HitCircle>(nullptr);

        pRect1->Width(3.0f);
        pRect1->Height(3.0f);

        pCircle->Radius(3.0f);

        pRect1->Rotation(test.Rotation1);

        pRigidBody1->HitShape(std::move(pRect1));
        pRigidBody2->HitShape(std::move(pCircle));

        pRigidBody1->Position(test.Origin1);
        pRigidBody2->Position(test.Origin2);

        pRigidBody2->Velocity({1.0f, 1.0f});

        m_World.Update(1.0);

        ASSERT_EQ(collisionOccurred, test.Result);
    }

    INSTANTIATE_TEST_SUITE_P(CircleCollisions, Circle_Physics_Tests, testing::ValuesIn(GetCircleTests()));
} // namespace Ignosi::Test
