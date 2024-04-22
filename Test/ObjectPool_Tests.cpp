#include <ObjectPool.hpp>
#include <RingBuffer.hpp>

#include <gtest/gtest.h>

using namespace Ignosi::Libraries;

namespace Ignosi::Test
{
    namespace
    {
        struct Data
        {
            Data(size_t value1, double value2)
                : Value1(value1)
                , Value2(value2)
            {
            }

            size_t Value1;
            double Value2;
        };

    } // namespace

    class ObjectPoolFixture : public testing::Test
    {
      protected:
        Containers::ObjectPool<Data> m_Pool;
    };

    TEST_F(ObjectPoolFixture, ValidateConstruction) {}

    TEST_F(ObjectPoolFixture, CreateDestroySingle)
    {
        auto obj = m_Pool.Create(1, 1.25);
        obj.reset();
        m_Pool.Process();
    }

    TEST_F(ObjectPoolFixture, CreateDestroyMany)
    {
        Containers::RingBuffer<Containers::PoolPointer<Data>, 64> objs;

        size_t val1 = 0;
        double val2 = 1;

        size_t val1_2 = 0;
        double val2_2 = 1;

        for (size_t i = 0; i < 5; ++i)
        {
            while (!objs.full())
            {
                objs.push(m_Pool.Create(val1++, val2++));
            }

            while (!objs.empty())
            {
                auto  obj  = objs.pop();
                Data& data = *obj.value();

                ASSERT_EQ(val1_2, obj.value()->Value1);
                ASSERT_EQ(val2_2, obj.value()->Value2);

                ASSERT_EQ(val1_2, (*obj.value()).Value1);
                ASSERT_EQ(val2_2, (*obj.value()).Value2);

                val1_2++;
                val2_2++;
            }

            m_Pool.Process();
        }
    }
} // namespace Ignosi::Test
