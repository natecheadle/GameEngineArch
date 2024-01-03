#include <Property_RO.hpp>
#include <Property_RW.hpp>

#include <gtest/gtest.h>

using namespace Ignosi::Libraries;

namespace Ignosi::Test
{
    class TestClass
    {
      public:
        TestClass()
            : AdvancedReadOnlyProp(this)
        {
            SimpleProp           = 0.0;
            AdvancedProp         = 0.0;
            ReadOnlyProp.m_Field = 2.0;
        }

        Property_RW<double, TestClass> SimpleProp;

        class : public Property_RW<double, TestClass>
        {
            friend TestClass;

          public:
            double& operator=(const double& value) override { return Property_RW<double, TestClass>::operator=(std::round(value)); }
            bool    operator==(const double& rhs) const override { return std::round(rhs) == std::round(m_Field); }
        } AdvancedProp;

        Property_RO<double, TestClass> ReadOnlyProp;

        class Prop : public Property_RO<double, TestClass, double>
        {
            friend TestClass;
            TestClass* m_Parent;
            Prop(TestClass* parent)
                : m_Parent(parent)
            {
            }

          public:
            double operator()() const override { return m_Parent->AdvancedProp() + m_Parent->SimpleProp(); }
            bool   operator==(const double& rhs) const override { return (*this)() == rhs; }

        } AdvancedReadOnlyProp;
    };

    TEST(PropertyFixture, ValidateGetSetSimple)
    {
        TestClass test;
        ASSERT_EQ(test.SimpleProp, 0.0);
        test.SimpleProp = 1.0;
        ASSERT_EQ(test.SimpleProp, 1.0);
    }

    TEST(PropertyFixture, ValidateGetSetAdvanced)
    {
        TestClass test;
        ASSERT_EQ(test.AdvancedProp, 0.0);
        test.AdvancedProp = 1.1;
        ASSERT_EQ(test.AdvancedProp, 1.2);
    }

    TEST(PropertyFixture, ValidateGetReadOnly)
    {
        TestClass test;
        ASSERT_EQ(test.ReadOnlyProp, 2.0);
    }

    TEST(PropertyFixture, ValidateGetReadOnlyAdvanced)
    {
        TestClass test;
        test.SimpleProp   = 2.0;
        test.AdvancedProp = 3.2;
        ASSERT_EQ(test.AdvancedReadOnlyProp, 5.0);
    }

} // namespace Ignosi::Test
