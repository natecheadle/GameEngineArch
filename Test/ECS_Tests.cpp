#include "EntityPointer.h"
#include "PoolMemoryBlock.hpp"
#include "System.h"
#include "Tag.h"
#include "World.h"

#include <gtest/gtest.h>

#include <memory>

namespace Ignosi::Test
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
        Modules::ECS::World<KinematicData> World;

        KinematicData init({Position({1.0, 2.0, 3.0}), Velocity({-1.0, 0.0, 1.0})});

        Modules::ECS::EntityPointer<KinematicData> entity = World.CreateEntity<KinematicData>();
        entity->Get<KinematicData>()                      = init;
        ASSERT_EQ(init, entity->Get<KinematicData>());
    }

    TEST(ECS_Tests, InitWorldAndSystem)
    {
        Modules::ECS::World<KinematicData> World;

        std::unique_ptr<TestSystem>                system = World.CreateSystem<TestSystem, KinematicData>();
        Modules::ECS::EntityPointer<KinematicData> entity = World.CreateEntity<KinematicData>();
        entity->Get<KinematicData>() = KinematicData({Position({1.0, 2.0, 3.0}), Velocity({-1.0, 0.0, 1.0})});

        World.Update();
        ASSERT_NO_THROW(system->Process(1.0));

        KinematicData expect({Position({0.0, 2.0, 4.0}), Velocity({-1.0, 0.0, 1.0})});

        ASSERT_EQ(entity->Get<KinematicData>(), expect);
    }

    TEST(ECS_Tests, ValidateBasicEntityCreateDelete)
    {
        Modules::ECS::World<KinematicData>                      World;
        std::vector<Modules::ECS::EntityPointer<KinematicData>> Entities;
        Entities.reserve(10);
        for (int i = 0; i < 12; ++i)
        {
            if (i % 3 == 0)
            {
                ASSERT_NO_THROW(Entities.clear());
            }
            else
            {
                for (int j = 0; j < 5; ++j)
                {
                    ASSERT_NO_THROW(Entities.push_back(World.CreateEntity<KinematicData>()));
                }
            }
            ASSERT_NO_THROW(World.Update());
        }
    }

    TEST(ECS_Tests, ValidateAdvancedEntityCreateDelete)
    {
        Modules::ECS::World<KinematicData>                      World;
        std::vector<Modules::ECS::EntityPointer<KinematicData>> Entities1;
        std::vector<Modules::ECS::EntityPointer<KinematicData>> Entities2;

        for (int i = 0; i < 12; ++i)
        {
            if (i % 4 == 0)
            {
                ASSERT_NO_THROW(Entities2.clear());
            }
            else if (i % 4 == 1 || i % 4 == 2)
            {
                for (int j = 0; j < 5; ++j)
                {
                    ASSERT_NO_THROW(Entities1.push_back(World.CreateEntity<KinematicData>()));
                }
                ASSERT_NO_THROW(Entities2.clear());
            }
            else
            {
                for (int j = 0; j < 5; ++j)
                {
                    ASSERT_NO_THROW(Entities2.push_back(World.CreateEntity<KinematicData>()));
                }
            }
            ASSERT_NO_THROW(World.Update());
        }
    }

    TEST(ECS_Tests, ValidateTag)
    {
        Modules::ECS::Tag tag  = Modules::ECS::Tag::Create("TestTag");
        auto              tag2 = tag;
        Modules::ECS::Tag tag3 = Modules::ECS::Tag::Create("TestTag");

        ASSERT_EQ(tag.ID(), tag2.ID());
        ASSERT_EQ(tag.Name(), tag2.Name());

        ASSERT_EQ(tag.ID(), tag3.ID());
        ASSERT_EQ(tag.Name(), tag3.Name());
    }

    TEST(ECS_Tests, ValidateAddRemoveTag)
    {

        Modules::ECS::World<KinematicData>                      World;
        std::vector<Modules::ECS::EntityPointer<KinematicData>> Entities;
        Modules::ECS::Tag                                       tag = Modules::ECS::Tag::Create("TestTag");

        for (int i = 0; i < 12; ++i)
        {
            if (i % 4 == 0)
            {
                ASSERT_NO_THROW(Entities.clear());
            }
            else if (i % 4 == 1)
            {
                for (int j = 0; j < 5; ++j)
                {
                    ASSERT_NO_THROW(Entities.push_back(World.CreateEntity<KinematicData>()));
                }
                std::sort(
                    Entities.begin(),
                    Entities.end(),
                    [](const Modules::ECS::EntityPointer<KinematicData>& lhs,
                       const Modules::ECS::EntityPointer<KinematicData>& rhs) { return lhs->ID() < rhs->ID(); });
            }
            else if (i % 4 == 2)
            {
                for (auto& entity : Entities)
                {
                    ASSERT_TRUE(World.AddTag(tag, entity));
                }
            }
            else
            {
                const auto& taggedEntities = World.GetEntitiesByTag(tag);
                ASSERT_EQ(taggedEntities.size(), Entities.size());
                for (size_t i = 0; i < taggedEntities.size(); ++i)
                {
                    ASSERT_EQ(taggedEntities[i], Entities[i]->ID());
                }
            }
            ASSERT_NO_THROW(World.Update());
        }
    }
} // namespace Ignosi::Test
