#include <StaticLinearMemoryBlock.hpp>
#include <StaticPoolMemoryBlock.hpp>
#include <StaticStackMemoryBlock.hpp>

#include <gtest/gtest.h>

namespace nate::Test {
    class TestObject {
        size_t m_Value;

      public:
        TestObject(size_t val)
            : m_Value(val)
        {
        }

        ~TestObject() { m_Value = 0; }

        size_t GetValue() const { return m_Value; }
    };

    TEST(StaticMemoryBlock_Tests, TestLinearMemBlock)
    {
        Modules::Memory::StaticLinearMemoryBlock<1024> memBlock;

        auto myObject = memBlock.MakeObject<TestObject, int>(10);
        ASSERT_EQ(10, myObject->GetValue());
        auto myObject1 = memBlock.MakeObject<TestObject, int>(20);
        ASSERT_EQ(20, myObject1->GetValue());
        auto myObject2 = memBlock.MakeObject<TestObject, int>(30);
        ASSERT_EQ(30, myObject2->GetValue());

        memBlock.Reset();
        ASSERT_EQ(0, memBlock.UsedSize());
    }

    TEST(StaticMemoryBlock_Tests, TestPoolMemBlock)
    {
        Modules::Memory::StaticPoolMemoryBlock<TestObject, 1024> memBlock;

        auto myObject = memBlock.MakeObject<int>(10);
        ASSERT_EQ(10, myObject->GetValue());
        {

            auto myObject1 = memBlock.MakeObject<int>(20);
            ASSERT_EQ(20, myObject1->GetValue());
            auto myObject2 = memBlock.MakeObject<int>(30);
            ASSERT_EQ(30, myObject2->GetValue());
        }
        ASSERT_EQ(sizeof(TestObject), memBlock.UsedSize());

        auto myObject1 = memBlock.MakeObject<int>(40);
        ASSERT_EQ(40, myObject1->GetValue());
        auto myObject2 = memBlock.MakeObject<int>(50);
        ASSERT_EQ(50, myObject2->GetValue());
    }

    TEST(StaticMemoryBlock_Tests, TestStackMemBlock)
    {
        Modules::Memory::StaticStackMemoryBlock<1024> memBlock;
        {
            auto myObject = memBlock.MakeObject<TestObject, int>(10);
            ASSERT_EQ(10, myObject->GetValue());
            auto myObject1 = memBlock.MakeObject<TestObject, int>(20);
            ASSERT_EQ(20, myObject1->GetValue());
            auto myObject2 = memBlock.MakeObject<TestObject, int>(30);
            ASSERT_EQ(30, myObject2->GetValue());

            memBlock.Release(2);
            ASSERT_EQ(sizeof(TestObject) + sizeof(size_t), memBlock.UsedSize());

            myObject1 = memBlock.MakeObject<TestObject, int>(40);
            ASSERT_EQ(40, myObject1->GetValue());
            myObject2 = memBlock.MakeObject<TestObject, int>(50);
            ASSERT_EQ(50, myObject2->GetValue());
        }
    }
} // namespace nate::Test