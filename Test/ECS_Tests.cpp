#include "PoolMemoryBlock.hpp"
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

    struct Velocity
    {
        float dX;
        float dY;
        float dZ;

        friend bool operator==(const Velocity& lhs, const Velocity& rhs) = default;
    };

    struct KinematicData
    {
        Position Pos;
        Velocity Vel;

        friend bool operator==(const KinematicData& lhs, const KinematicData& rhs) = default;
    };

    class TestEntity : public Modules::ECS::Entity<KinematicData>
    {
      public:
        TestEntity(Modules::Memory::pool_pointer<KinematicData>&& val)
            : Modules::ECS::Entity<KinematicData>(std::move(val))
        {
        }

        KinematicData&       Kinematic() { return Modules::ECS::Entity<KinematicData>::Get<KinematicData>(); }
        const KinematicData& Kinematic() const { return Modules::ECS::Entity<KinematicData>::Get<KinematicData>(); }

        Position& Pos() { return Modules::ECS::Entity<KinematicData>::Get<KinematicData>().Pos; }
        Velocity& Vel() { return Modules::ECS::Entity<KinematicData>::Get<KinematicData>().Vel; }

        const Position& Pos() const { return Modules::ECS::Entity<KinematicData>::Get<KinematicData>().Pos; }
        const Velocity& Vel() const { return Modules::ECS::Entity<KinematicData>::Get<KinematicData>().Vel; }
    };

    TEST(ECS_Tests, InitWorldAndEntity)
    {
        Modules::ECS::World<KinematicData> world;

        KinematicData init({Position({1.0, 2.0, 3.0}), Velocity({-1.0, 0.0, 1.0})});

        TestEntity entity = world.CreateEntity<TestEntity>(KinematicData(init));
        ASSERT_EQ(init, entity.Kinematic());
    }
} // namespace nate::Test
