#include "PoolPointer.hpp"

#include <Entity.hpp>
#include <System.hpp>
#include <World.hpp>

#include <gtest/gtest.h>

#include <chrono>
#include <memory>

using namespace Ignosi::Libraries;
using namespace std::chrono_literals;

namespace Ignosi::Test::ECS
{
    struct ComponentData1
    {
        double X;
        double Y;
        double Z;

        friend bool operator==(const ComponentData1& lhs, const ComponentData1& rhs) = default;
    };

    struct ComponentData2
    {
        int CountOfHits;
        int CountOfMisses;

        friend bool operator==(const ComponentData2& lhs, const ComponentData2& rhs) = default;
    };

    class System1 : public Libraries::ECS::System<ComponentData1>
    {
      public:
        void Update(std::chrono::milliseconds delta) override
        {
            for (ComponentData1& data : ComponentPool())
            {
                data.X += delta / 1.0s;
                data.Y += delta * 2 / 1.0s;
                data.Z += delta * 4 / 1.0s;
            }
        }
    };

    class System2 : public Libraries::ECS::System<ComponentData2>
    {
      public:
        void Update(std::chrono::milliseconds delta) override
        {

            for (ComponentData2& data : ComponentPool())
            {

                if (data.CountOfHits > 5)
                {
                    data.CountOfHits = 5;
                }
                if (data.CountOfMisses > 10)
                {
                    data.CountOfMisses = 10;
                }
            }
        }
    };

    using TestWorld     = Libraries::ECS::World<ComponentData1, ComponentData2>;
    using TestEntityPtr = Containers::PoolPointer<Libraries::ECS::Entity<ComponentData1, ComponentData2>>;

    class ECSFixture : public testing::Test
    {
      protected:
        TestWorld m_World;

      public:
        ECSFixture()
        {
            m_World.Register<ComponentData1>(std::make_unique<System1>());
            m_World.Register<ComponentData2>(std::make_unique<System2>());
        }
    };
} // namespace Ignosi::Test::ECS
