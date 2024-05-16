#include "ECS_Tests.hpp"

#include <gtest/gtest.h>

#include <array>
#include <memory>

namespace Ignosi::Test::ECS
{
    namespace
    {
        struct TestData
        {
            std::array<float, 16> Data;
        };
    } // namespace
      //
    class ResourceManagerFixture : public ECSFixture
    {
      protected:
        std::string m_ResourceName = "MyResource";
    };

    TEST_F(ResourceManagerFixture, ValidateCreateResource)
    {
        m_World.Resources().CreateResource(m_ResourceName, std::make_unique<TestData>());
    }

    TEST_F(ResourceManagerFixture, ValidateGetResource)
    {

        auto  pResource = m_World.Resources().CreateResource(m_ResourceName, std::make_unique<TestData>());
        float i         = 0;
        for (float& value : pResource->get()->Data)
        {
            value = i++;
        }

        auto gotResource = m_World.Resources().GetResource<TestData>(m_ResourceName);
        ASSERT_EQ(pResource->get(), gotResource->get());

        i = 0;

        for (const float& value : gotResource->get()->Data)
        {
            ASSERT_EQ(value, i++);
        }
    }

    TEST_F(ResourceManagerFixture, ValidateDeleteResource)
    {
        m_World.Resources().CreateResource(m_ResourceName, std::make_unique<TestData>());

        ASSERT_NE(m_World.Resources().GetResource<TestData>(m_ResourceName), nullptr);
        ASSERT_EQ(m_World.Resources().GetResource<ComponentData1>(m_ResourceName), nullptr);

        ASSERT_TRUE(m_World.Resources().DestroyResource(m_ResourceName));
        ASSERT_EQ(m_World.Resources().GetResource<TestData>(m_ResourceName), nullptr);
        ASSERT_FALSE(m_World.Resources().DestroyResource(m_ResourceName));
    }
} // namespace Ignosi::Test::ECS
