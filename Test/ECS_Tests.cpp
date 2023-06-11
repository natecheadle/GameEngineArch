#include "PoolMemoryBlock.hpp"
#include "System.h"
#include "World.h"

#include <gtest/gtest.h>

#include <cmath>
#include <memory>

namespace nate::Test
{
    struct Color
    {
        float Red;
        float Green;
        float Blue;

        friend bool operator==(const Color& lhs, const Color& rhs) = default;
    };

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
        Position Position;
        Velocity Velocity;

        friend bool operator==(const KinematicData& lhs, const KinematicData& rhs) = default;
    };

    class TestEntity : public Modules::ECS::Entity<KinematicData, Color>
    {
        using BASE = Modules::ECS::Entity<struct KinematicData, struct Color>;

      public:
        TestEntity(Modules::Memory::pool_pointer<KinematicData>&& kin, Modules::Memory::pool_pointer<Color>&& color)
            : BASE(std::move(kin), std::move(color))
        {
        }

        KinematicData& Kinematic() { return Modules::ECS::Entity<KinematicData, struct Color>::Get<KinematicData>(); }
        const KinematicData& Kinematic() const { return BASE::Get<KinematicData>(); }

        struct Position& Position() { return BASE::Get<KinematicData>().Position; }
        struct Velocity& Velocity() { return BASE::Get<KinematicData>().Velocity; }

        const struct Position& Position() const { return BASE::Get<KinematicData>().Position; }
        const struct Velocity& Velocity() const { return BASE::Get<KinematicData>().Velocity; }

        struct Color&       Color() { return BASE::Get<struct Color>(); }
        const struct Color& Color() const { return BASE::Get<struct Color>(); }
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
                val.Position.X += val.Velocity.dX * timeStep;
                val.Position.Y += val.Velocity.dY * timeStep;
                val.Position.Z += val.Velocity.dZ * timeStep;
            }
        }
    };

    class TestSystem2 : public Modules::ECS::System<Color>
    {
        float m_TotalTime;

      public:
        TestSystem2(Modules::Memory::PoolMemoryBlock<Color>* pPool)
            : Modules::ECS::System<Color>(pPool)
        {
        }

        void Process(float timeStep)
        {
            m_TotalTime += timeStep;
            m_TotalTime = std::fmod(m_TotalTime, 1.0f);
            auto& pool  = GetPool<Color>();
            for (auto& val : pool)
            {
                val.Red *= sin(m_TotalTime);
            }
        }
    };

    TEST(ECS_Tests, InitWorldAndEntity)
    {
        Modules::ECS::World<KinematicData, Color> world;

        KinematicData               init({Position({1.0, 2.0, 3.0}), Velocity({-1.0, 0.0, 1.0})});
        Color                       col({1.0, 1.0, 1.0});
        std::unique_ptr<TestEntity> entity = world.CreateEntity<TestEntity>(KinematicData(init), Color(col));
        ASSERT_EQ(init, entity->Kinematic());
        ASSERT_EQ(col, entity->Color());
    }

    TEST(ECS_Tests, InitWorldAndSystem)
    {
        Modules::ECS::World<KinematicData, Color> world;
        ;

        std::unique_ptr<TestSystem> system = world.CreateSystem<TestSystem, KinematicData>();
        std::unique_ptr<TestEntity> entity = world.CreateEntity<TestEntity>(
            KinematicData({Position({1.0, 2.0, 3.0}), Velocity({-1.0, 0.0, 1.0})}),
            Color({1.0, 1.0, 1.0}));

        ASSERT_NO_THROW(system->Process(1.0));

        KinematicData expect({Position({0.0, 2.0, 4.0}), Velocity({-1.0, 0.0, 1.0})});
        Color         expectCol({std::sin(1.0f), std::sin(1.0f), std::sin(1.0f)});

        ASSERT_EQ(entity->Kinematic(), expect);
        ASSERT_EQ(expectCol, expectCol);
    }
} // namespace nate::Test
