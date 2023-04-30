#include "World.h"

#include <gtest/gtest.h>

namespace nate::Test
{
    struct Position
    {
        float X;
        float Y;
        float Z;

        friend bool operator==(const Position& lhs, const Position& rhs) = default;
    };

    TEST(ECS_Tests, InitWorldAndEntity)
    {
        Modules::ECS::World<Position> world;

        Position init(1.0, 2.0, 3.0);

        Modules::ECS::Entity<Position> entity = world.CreateEntity(Position(init));
        ASSERT_EQ(init, entity.Get<Position>());
    }
} // namespace nate::Test
