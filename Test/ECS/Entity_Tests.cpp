#include "ECS_Tests.hpp"

namespace Ignosi::Test::ECS
{
    class EntityFixture : public ECSFixture
    {
      protected:
        TestEntityPtr m_Entity;

      public:
        void SetUp() { m_Entity = m_World.CreateEntity(); }
    };

    TEST_F(EntityFixture, ValidateCreation) {}

    TEST_F(EntityFixture, AssignComponent)
    {
        m_Entity->Set(ComponentData1());
    }

    TEST_F(EntityFixture, UpdateComponent)
    {

        m_Entity->Set(ComponentData1());
        m_Entity->Get<ComponentData1>().X = 10.0;
        m_Entity->Get<ComponentData1>().Y = 20.0;
        m_Entity->Get<ComponentData1>().Z = 30.0;

        ASSERT_EQ(m_Entity->Get<ComponentData1>(), ComponentData1(10, 20, 30));
    }

} // namespace Ignosi::Test::ECS
