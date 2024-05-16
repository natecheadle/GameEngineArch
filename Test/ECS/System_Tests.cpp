#include "ECS_Tests.hpp"

namespace Ignosi::Test::ECS
{
    class SystemFixture : public ECSFixture
    {
      protected:
        std::vector<TestEntityPtr> m_Entities;

      public:
        void SetUp()
        {
            for (int i = 0; i < 10; ++i)
            {
                TestEntityPtr  newEntity = m_World.CreateEntity();
                ComponentData1 componentData{i * 1.0, i * 2.0, i * 3.0};
                newEntity->Set(componentData);
                if (i % 2 == 0)
                {
                    ComponentData2 componentData2{i, i * 2};
                    newEntity->Set(componentData2);
                }
                m_Entities.push_back(std::move(newEntity));
            }
        }
    };

    TEST_F(SystemFixture, ValidateCreation) {}

    TEST_F(SystemFixture, ValidateUpdate)
    {
        m_World.Update(1s);

        for (size_t i = 0; i < m_Entities.size(); ++i)
        {
            ComponentData1 exp{i * 1.0 + 1.0, i * 2.0 + 2.0, i * 3.0 + 4.0};
            ASSERT_EQ(exp, m_Entities[i]->Get<ComponentData1>());
            if (i % 2 == 0)
            {
                ComponentData2 exp{(int)i, (int)i * 2};
                if (i > 5)
                {
                    exp.CountOfHits   = 5;
                    exp.CountOfMisses = 10;
                }

                ASSERT_EQ(exp, m_Entities[i]->Get<ComponentData2>());
            }
        }
    }
} // namespace Ignosi::Test::ECS
