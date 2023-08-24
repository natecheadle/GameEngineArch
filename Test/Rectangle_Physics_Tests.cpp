#include <HitRectangle.h>
#include <HitShape.h>
#include <LinearAlgebra/Vector2.hpp>
#include <PhysicsSystem.h>
#include <PoolMemoryBlock.hpp>
#include <RigidBody2D.h>
#include <Units/Radian.hpp>

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
    struct RectangleCollisionInputData
    {
        RectangleCollisionInputData(
            const Vector2<float>& origin1,
            float                 rotation1,
            const Vector2<float>& origin2,
            float                 rotation2,
            bool                  result)
        {
            Origin1   = origin1;
            Rotation1 = rotation1;
            Origin2   = origin2;
            Rotation2 = rotation2;
            Result    = result;
        }

        Vector2<float> Origin1;
        float          Rotation1;
        Vector2<float> Origin2;
        float          Rotation2;
        bool           Result;
    };

    namespace
    {
        std::vector<RectangleCollisionInputData> GetRectangleTests()
        {
            std::vector<RectangleCollisionInputData> tests;

            tests.push_back(RectangleCollisionInputData({0.0f, 0.0f}, 0.0f, {1.0f, 1.0f}, 0.0f, false));
            tests.push_back(RectangleCollisionInputData({0.0f, 0.0f}, 0.0f, {0.5f, 0.5f}, 0.0f, true));
            tests.push_back(RectangleCollisionInputData({0.0f, 0.0f}, 0.0f, {3.0f, 3.0f}, 0.0f, false));
            tests.push_back(RectangleCollisionInputData({0.0f, 0.0f}, M_PI_4, {0.5f, 0.5f}, 0.0f, false));
            tests.push_back(RectangleCollisionInputData({0.5f, 0.5f}, 0.0, {0.0f, 0.0f}, -M_PI_4, false));

            return tests;
        }
    } // namespace

    class Rectangle_Physics_Tests : public testing::TestWithParam<RectangleCollisionInputData>
    {
      protected:
        Memory::PoolMemoryBlock<Physics::RigidBody2D> m_RigidBodies;
        Physics::PhysicsSystem                        m_PhysicsSystem;

      public:
        Rectangle_Physics_Tests()
            : m_PhysicsSystem(&m_RigidBodies)
        {
        }
    };

    TEST_P(Rectangle_Physics_Tests, ValidateRectangleCollision)
    {
        auto test = GetParam();

        bool collisionOccurred = false;
        auto onCollision       = [&](const Physics::RigidBody2D&) { collisionOccurred = true; };

        auto pRigidBody1 = m_RigidBodies.CreateObject();
        auto pRigidBody2 = m_RigidBodies.CreateObject();

        pRigidBody2->AttachOnCollision(onCollision);
        pRigidBody1->AttachOnCollision(onCollision);

        std::unique_ptr<Physics::HitRectangle> pRect1 = std::make_unique<Physics::HitRectangle>(nullptr);
        std::unique_ptr<Physics::HitRectangle> pRect2 = std::make_unique<Physics::HitRectangle>(nullptr);

        pRect1->Width(3.0f);
        pRect2->Width(3.0f);

        pRect1->Height(3.0f);
        pRect2->Height(3.0f);

        pRect1->Rotation(test.Rotation1);
        pRect2->Rotation(test.Rotation2);

        pRigidBody1->HitShape(std::move(pRect1));
        pRigidBody2->HitShape(std::move(pRect2));

        pRigidBody1->Position(test.Origin1);
        pRigidBody2->Position(test.Origin2);

        pRigidBody2->Velocity({1.0f, 1.0f});

        m_PhysicsSystem.Update(std::chrono::seconds(1));

        pRigidBody1.reset();
        pRigidBody2.reset();

        ASSERT_EQ(collisionOccurred, test.Result);
    }

    INSTANTIATE_TEST_SUITE_P(RectangleCollisions, Rectangle_Physics_Tests, testing::ValuesIn(GetRectangleTests()));
} // namespace Ignosi::Test
