#include "HitRectangle.h"
#include "HitShape.h"
#include "PoolMemoryBlock.hpp"
#include "RigidBody2D.h"

#include <PhysicsSystem.h>

#include <gtest/gtest.h>

#include <chrono>
#include <memory>
using namespace Ignosi::Modules;

namespace Ignosi::Test
{
    class Physics_Tests : public testing::Test
    {
      protected:
        Memory::PoolMemoryBlock<Physics::RigidBody2D> m_RigidBodies;
        Physics::PhysicsSystem                        m_PhysicsSystem;

      public:
        Physics_Tests()
            : m_PhysicsSystem(&m_RigidBodies)
        {
        }
    };

    TEST_F(Physics_Tests, ValidateRectangleCollision)
    {
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
        pRect2->Width(3.0f);

        pRect1->Rotation(0.0f);
        pRect2->Rotation(0.0f);

        pRigidBody1->HitShape(std::move(pRect1));
        pRigidBody2->HitShape(std::move(pRect2));

        pRigidBody1->Position({0.0f, 0.0f});
        pRigidBody2->Position({0.0f, 0.0f});

        pRigidBody1->Velocity({1.0f, 1.0f});

        m_PhysicsSystem.Update(std::chrono::seconds(1));

        pRigidBody1.reset();
        pRigidBody2.reset();

        ASSERT_TRUE(collisionOccurred);
    }
} // namespace Ignosi::Test
