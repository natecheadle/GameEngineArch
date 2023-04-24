#include "ComponentPool.h"

#include <ComponentPool.h>

#include <gtest/gtest.h>

namespace nate::Test
{
    struct Position
    {
        float X;
        float Y;
        float Z;
    };

    TEST(ECS_Tests, ComponentPoolInitValidation)
    {
        Modules::ECS::ComponentPool<Position> pool(64);
    }

    TEST(ECS_Tests, ComponentPoolAddValidation)
    {
        Modules::ECS::ComponentPool<Position> pool(64);
        pool.AddComponent(0, {0.0, 0.0, 0.0});
        auto pComp = pool.GetComponent(0);
    }

    TEST(ECS_Tests, ComponentPoolAddRemoveValidation)
    {
        Modules::ECS::ComponentPool<Position> pool(64);
        pool.AddComponent(0, {0.0, 0.0, 0.0});
        auto pComp = pool.GetComponent(0);
        ASSERT_NE(nullptr, pComp.get());
        pool.RemoveComponent(0);
        pComp = pool.GetComponent(0);
        ASSERT_EQ(nullptr, pComp.get());
    }
} // namespace nate::Test
