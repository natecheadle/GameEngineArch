#include <StaticFreeMemoryBlock.hpp>
#include <StaticLinearMemoryBlock.hpp>
#include <StaticPoolMemoryBlock.hpp>
#include <StaticStackMemoryBlock.hpp>

#include <gtest/gtest.h>

#include <array>

namespace nate::Test {
    namespace {
        constexpr size_t MemorySize = 1024;
        std::uint8_t     MemoryBuffer[1024];
    } // namespace

    class TestObject {
        size_t m_Value;
        size_t m_Value1;

      public:
        TestObject(size_t val)
            : m_Value(val)
            , m_Value1(val * 2)
        {
        }

        ~TestObject() { m_Value = 0; }

        size_t GetValue() const { return m_Value; }
        size_t GetValue1() const { return m_Value1; }
    };

    TEST(StaticMemoryBlock_Tests, TestLinearMemBlock)
    {
        Modules::Memory::StaticLinearMemoryBlock<MemoryBuffer, MemorySize> memBlock;

        auto myObject = memBlock.MakeObject<TestObject, int>(10);
        ASSERT_EQ(10, myObject->GetValue());
        ASSERT_EQ(20, myObject->GetValue1());
        auto myObject1 = memBlock.MakeObject<TestObject, int>(20);
        ASSERT_EQ(20, myObject1->GetValue());
        ASSERT_EQ(40, myObject1->GetValue1());
        auto myObject2 = memBlock.MakeObject<TestObject, int>(30);
        ASSERT_EQ(30, myObject2->GetValue());
        ASSERT_EQ(60, myObject2->GetValue1());

        memBlock.Reset();
        ASSERT_EQ(0, memBlock.UsedSize());
    }

    TEST(StaticMemoryBlock_Tests, TestPoolMemBlock)
    {
        Modules::Memory::StaticPoolMemoryBlock<TestObject, MemoryBuffer, MemorySize> memBlock;

        auto myObject = memBlock.MakeObject<int>(10);
        ASSERT_EQ(10, myObject->GetValue());
        {

            auto myObject1 = memBlock.MakeObject<int>(20);
            ASSERT_EQ(20, myObject1->GetValue());
            ASSERT_EQ(40, myObject1->GetValue1());
            auto myObject2 = memBlock.MakeObject<int>(30);
            ASSERT_EQ(30, myObject2->GetValue());
            ASSERT_EQ(60, myObject2->GetValue1());
        }
        ASSERT_EQ(sizeof(TestObject), memBlock.UsedSize());

        auto myObject1 = memBlock.MakeObject<int>(40);
        ASSERT_EQ(40, myObject1->GetValue());
        ASSERT_EQ(80, myObject1->GetValue1());
        auto myObject2 = memBlock.MakeObject<int>(50);
        ASSERT_EQ(50, myObject2->GetValue());
        ASSERT_EQ(100, myObject2->GetValue1());
    }

    TEST(StaticMemoryBlock_Tests, TestStackMemBlock)
    {
        Modules::Memory::StaticStackMemoryBlock<MemoryBuffer, MemorySize> memBlock;
        {
            auto myObject = memBlock.MakeObject<TestObject, int>(10);
            ASSERT_EQ(10, myObject->GetValue());
            ASSERT_EQ(20, myObject->GetValue1());
            auto myObject1 = memBlock.MakeObject<TestObject, int>(20);
            ASSERT_EQ(20, myObject1->GetValue());
            ASSERT_EQ(40, myObject1->GetValue1());
            auto myObject2 = memBlock.MakeObject<TestObject, int>(30);
            ASSERT_EQ(30, myObject2->GetValue());
            ASSERT_EQ(60, myObject2->GetValue1());

            memBlock.Release(2);
            ASSERT_EQ(sizeof(TestObject) + sizeof(size_t), memBlock.UsedSize());

            myObject1 = memBlock.MakeObject<TestObject, int>(40);
            ASSERT_EQ(40, myObject1->GetValue());
            ASSERT_EQ(80, myObject1->GetValue1());
            myObject2 = memBlock.MakeObject<TestObject, int>(50);
            ASSERT_EQ(50, myObject2->GetValue());
            ASSERT_EQ(100, myObject2->GetValue1());
        }
    }

    TEST(StaticMemoryBlock_Tests, TestFreeMemoryBlock)
    {
        Modules::Memory::StaticFreeMemoryBlock<MemoryBuffer, MemorySize> memBlock;
        {
            auto myObject = memBlock.MakeObject<TestObject, int>(10);
            {
                ASSERT_EQ(10, myObject->GetValue());
                ASSERT_EQ(20, myObject->GetValue1());
                auto myObject1 = memBlock.MakeObject<TestObject, int>(20);
                ASSERT_EQ(20, myObject1->GetValue());
                ASSERT_EQ(40, myObject1->GetValue1());
                auto myObject2 = memBlock.MakeObject<TestObject, int>(30);
                ASSERT_EQ(30, myObject2->GetValue());
                ASSERT_EQ(60, myObject2->GetValue1());
            }
            ASSERT_EQ(sizeof(TestObject), memBlock.UsedSize());

            auto myObject1 = memBlock.MakeObject<TestObject, int>(40);
            ASSERT_EQ(40, myObject1->GetValue());
            ASSERT_EQ(80, myObject1->GetValue1());
            auto myObject2 = memBlock.MakeObject<TestObject, int>(50);
            ASSERT_EQ(50, myObject2->GetValue());
            ASSERT_EQ(100, myObject2->GetValue1());
        }
    }
} // namespace nate::Test