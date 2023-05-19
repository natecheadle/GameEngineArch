#include "PoolMemoryBlock.hpp"
#include "System.h"
#include "World.h"

#include <gtest/gtest.h>

#include <memory>

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

    class TestSystem : public Modules::ECS::System<KinematicData>
    {
      public:
        TestSystem(Modules::Memory::PoolMemoryBlock<KinematicData>* pPool)
            : Modules::ECS::System<KinematicData>(pPool)
        {
        }

        void Process(float timeStep)
        {
            auto& pool = GetPool<KinematicData>();
            for (auto& val : pool)
            {
                val.Pos.X += val.Vel.dX * timeStep;
                val.Pos.Y += val.Vel.dY * timeStep;
                val.Pos.Z += val.Vel.dZ * timeStep;
            }
        }
    };

    TEST(ECS_Tests, InitWorldAndEntity)
    {
        Modules::ECS::World<KinematicData> world;

        KinematicData init({Position({1.0, 2.0, 3.0}), Velocity({-1.0, 0.0, 1.0})});

        TestEntity entity = world.CreateEntity<TestEntity>(KinematicData(init));
        ASSERT_EQ(init, entity.Kinematic());
    }

    TEST(ECS_Tests, InitWorldAndSystem)
    {
        Modules::ECS::World<KinematicData> world;
        ;

        std::unique_ptr<TestSystem> system = world.CreateSystem<TestSystem, KinematicData>();
        TestEntity                  entity = world.CreateEntity<TestEntity>(
            KinematicData(KinematicData({Position({1.0, 2.0, 3.0}), Velocity({-1.0, 0.0, 1.0})})));

        ASSERT_NO_THROW(system->Process(1.0));

        KinematicData expect({Position({0.0, 2.0, 4.0}), Velocity({-1.0, 0.0, 1.0})});

        ASSERT_EQ(entity.Kinematic(), expect);
    }
} // namespace nate::Test
