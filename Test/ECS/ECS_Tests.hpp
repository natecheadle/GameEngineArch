#include "ECSObject.hpp"
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

    struct ComponentData3
    {
        double Value1;

        friend bool operator==(const ComponentData3& lhs, const ComponentData3& rhs) = default;
    };

    class System1 : public Libraries::ECS::System<ComponentData1>
    {
      public:
        void Update(std::chrono::milliseconds delta) override
        {
            for (Libraries::ECS::Component<ComponentData1>& data : ComponentPool())
            {
                data.Data().X += delta / 1.0s;
                data.Data().Y += delta * 2 / 1.0s;
                data.Data().Z += delta * 4 / 1.0s;
            }
        }
    };

    class System2 : public Libraries::ECS::System<ComponentData2>
    {
      public:
        void Update(std::chrono::milliseconds delta) override
        {

            for (Libraries::ECS::Component<ComponentData2>& data : ComponentPool())
            {

                if (data.Data().CountOfHits > 5)
                {
                    data.Data().CountOfHits = 5;
                }
                if (data.Data().CountOfMisses > 10)
                {
                    data.Data().CountOfMisses = 10;
                }
            }
        }
    };

    class DependentSystem : public Libraries::ECS::System<ComponentData3>
    {
        System1* m_pDependentSystem;

      public:
        DependentSystem(System1* pDependentSystem)
            : m_pDependentSystem(pDependentSystem)
        {
        }

        void Update(std::chrono::milliseconds delta) override
        {
            for (Libraries::ECS::Component<ComponentData3>& data : ComponentPool())
            {
                const Libraries::ECS::Component<ComponentData1>* pDependent = m_pDependentSystem->GetEntityComponent(data.EntityID());
                if (pDependent)
                {
                    data.Data().Value1 = pDependent->Data().X;
                }
            }
        }
    };

    using TestWorld     = Libraries::ECS::World<ComponentData1, ComponentData2, ComponentData3>;
    using TestEntityPtr = Libraries::ECS::ECSObject<Libraries::ECS::Entity<ComponentData1, ComponentData2, ComponentData3>>;

    class ECSFixture : public testing::Test
    {
      protected:
        TestWorld m_World;

      public:
        ECSFixture()
        {
            m_World.Register<ComponentData1>(std::make_unique<System1>());
            m_World.Register<ComponentData2>(std::make_unique<System2>());

            System1* pSystem1 = m_World.GetSystem<System1, ComponentData1>();
            m_World.Register<ComponentData3>(std::make_unique<DependentSystem>(pSystem1));
        }
        virtual ~ECSFixture() = default;
    };
} // namespace Ignosi::Test::ECS
