#include "ComponentPointer.h"
#include "EntityPointer.h"
#include "IComponent.h"
#include "LinearAlgebra/Vector3.hpp"
#include "PoolMemoryBlock.hpp"
#include "System.h"
#include "Tag.h"
#include "World.h"

#include <gtest/gtest.h>

#include <cmath>
#include <memory>

namespace Ignosi::Test
{
    class KinematicData : Modules::ECS::IComponent
    {
      public:
        Modules::Vector3<float> Pos;
        Modules::Vector3<float> Vel;
        friend bool operator==(const KinematicData& lhs, const KinematicData& rhs) { return lhs.Pos == rhs.Pos && lhs.Vel == rhs.Vel; }

        const Modules::ECS::Tag& Tag() const override
        {
            static const Modules::ECS::Tag s_tag = Modules::ECS::Tag::Create("KinematicData");
            return s_tag;
        }
    };

    class TestSystem : public Modules::ECS::System<KinematicData>
    {
        Modules::ECS::Tag m_TestTag;

      public:
        TestSystem(Modules::ECS::ComponentPool<KinematicData>* pPool)
            : Modules::ECS::System<KinematicData>(pPool)
        {
        }

        std::string_view Name() const override { return "Test System"; }
        std::uint32_t    Priority() const override { return 0; }
        void             Update(double dt) override
        {
            auto&       pool     = GetPool<KinematicData>();
            const auto& entities = World()->GetEntitiesByTag(Modules::ECS::ComponentPool<KinematicData>::ComponentTag());
            for (const auto& enitity : entities)
            {
                assert(pool.HasComponent(World()->GetEntity(enitity)));
                auto& data = pool.GetComponent(World()->GetEntity(enitity));
                data.Pos += data.Vel * (float)dt;
            }
        }
    };

    TEST(ECS_Tests, InitWorldAndEntity)
    {
        Modules::ECS::World<KinematicData> World;

        KinematicData init;
        init.Pos = Modules::Vector3<float>({1.0, 2.0, 3.0});
        init.Vel = Modules::Vector3<float>({-1.0, 0.0, 1.0});

        Modules::ECS::EntityPointer<KinematicData> entity = World.CreateEntity<KinematicData>(init);

        ASSERT_EQ(init, *(entity->GetComponent<KinematicData>()));
    }

    TEST(ECS_Tests, InitWorldAndSystem)
    {
        Modules::ECS::World<KinematicData> World;

        KinematicData init;
        init.Pos = Modules::Vector3<float>({1.0, 2.0, 3.0});
        init.Vel = Modules::Vector3<float>({-1.0, 0.0, 1.0});

        TestSystem*                                system = World.CreateSystem<TestSystem, KinematicData>();
        Modules::ECS::EntityPointer<KinematicData> entity = World.CreateEntity<KinematicData>(init);

        ASSERT_NO_THROW(World.Update(1.0));

        KinematicData expected;
        expected.Pos = Modules::Vector3<float>({0.0, 2.0, 4.0});
        expected.Vel = Modules::Vector3<float>({-1.0, 0.0, 1.0});
        ASSERT_EQ(*(entity->GetComponent<KinematicData>()), expected);
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
            ASSERT_NO_THROW(World.Update(1.0));
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
            ASSERT_NO_THROW(World.Update(1.0));
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
                    [](const Modules::ECS::EntityPointer<KinematicData>& lhs, const Modules::ECS::EntityPointer<KinematicData>& rhs) {
                        return lhs->ID() < rhs->ID();
                    });
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
            ASSERT_NO_THROW(World.Update(1.0));
        }
    }
} // namespace Ignosi::Test
