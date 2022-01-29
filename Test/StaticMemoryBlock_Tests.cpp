#include <StaticFreeMemoryBlock.hpp>
#include <StaticLinearMemoryBlock.hpp>
#include <StaticPoolMemoryBlock.hpp>
#include <StaticStackMemoryBlock.hpp>

#include <gtest/gtest.h>

#include <array>
#include <cmath>

namespace nate::Test {
    namespace {
        constexpr size_t MemorySize = 1024;
        std::uint8_t     MemoryBuffer[1024];
    } // namespace

    template <size_t SIZE>
    class TestObject {
        std::array<size_t, SIZE> m_Value;

      public:
        TestObject(size_t val)
        {
            for (size_t i = 1; i <= m_Value.size(); ++i)
            {
                m_Value.at(i - 1) = val * (1 << (i - 1));
            }
        }

        ~TestObject() {}

        size_t                          GetValue(size_t loc) const { return m_Value.at(loc); }
        const std::array<size_t, SIZE>& GetValue() const { return m_Value; }

        static void Validate(size_t initVal, const TestObject& object)
        {
            const std::array<size_t, SIZE>& privArray = object.GetValue();
            for (size_t i = 0; i < privArray.size(); ++i)
            {
                ASSERT_EQ(privArray.at(i), object.GetValue(i));
                ASSERT_EQ(object.GetValue(i), std::pow(2, i) * initVal);
            }
        }
    };

    TEST(StaticMemoryBlock_Tests, TestLinearMemBlock)
    {
        Modules::Memory::StaticLinearMemoryBlock<MemoryBuffer, MemorySize> memBlock;

        auto myObject = memBlock.MakeObject<TestObject<2>, size_t>(10);
        myObject->Validate(10, *myObject);
        auto myObject1 = memBlock.MakeObject<TestObject<4>, size_t>(20);
        myObject1->Validate(20, *myObject1);
        auto myObject2 = memBlock.MakeObject<TestObject<8>, size_t>(30);
        myObject2->Validate(30, *myObject2);

        memBlock.Reset();
        ASSERT_EQ(0, memBlock.UsedSize());

        auto myObject4 = memBlock.MakeObject<TestObject<1024>, size_t>(1);
        ASSERT_EQ(nullptr, myObject4);
    }

    TEST(StaticMemoryBlock_Tests, TestPoolMemBlock)
    {
        Modules::Memory::StaticPoolMemoryBlock<TestObject<2>, MemoryBuffer, MemorySize> memBlock;

        auto myObject = memBlock.MakeObject<size_t>(10);
        myObject->Validate(10, *myObject);
        {

            auto myObject1 = memBlock.MakeObject<size_t>(20);
            myObject1->Validate(20, *myObject1);
            auto myObject2 = memBlock.MakeObject<size_t>(30);
            myObject2->Validate(30, *myObject2);
        }
        ASSERT_EQ(sizeof(TestObject<2>), memBlock.UsedSize());

        auto myObject1 = memBlock.MakeObject<size_t>(40);
        myObject1->Validate(40, *myObject1);
        auto myObject2 = memBlock.MakeObject<size_t>(50);
        myObject2->Validate(50, *myObject2);
    }

    TEST(StaticMemoryBlock_Tests, TestStackMemBlock)
    {
        Modules::Memory::StaticStackMemoryBlock<MemoryBuffer, MemorySize> memBlock;
        {
            auto myObject = memBlock.MakeObject<TestObject<2>, size_t>(10);
            myObject->Validate(10, *myObject);
            auto myObject1 = memBlock.MakeObject<TestObject<2>, size_t>(20);
            myObject1->Validate(20, *myObject1);
            auto myObject2 = memBlock.MakeObject<TestObject<2>, size_t>(30);
            myObject2->Validate(30, *myObject2);

            memBlock.Release(2);
            ASSERT_EQ(sizeof(TestObject<2>) + sizeof(size_t), memBlock.UsedSize());

            myObject1 = memBlock.MakeObject<TestObject<2>, size_t>(40);
            myObject1->Validate(40, *myObject1);
            myObject2 = memBlock.MakeObject<TestObject<2>, size_t>(50);
            myObject2->Validate(50, *myObject2);

            myObject->Validate(10, *myObject);

            memBlock.Release(4);

            auto myObject4 = memBlock.MakeObject<TestObject<1024>, size_t>(1);
            ASSERT_EQ(nullptr, myObject4);
        }
    }

    TEST(StaticMemoryBlock_Tests, TestFreeMemoryBlock)
    {
        Modules::Memory::StaticFreeMemoryBlock<MemoryBuffer, MemorySize> memBlock;
        {
            auto myObject = memBlock.MakeObject<TestObject<4>, size_t>(10);
            {
                myObject->Validate(10, *myObject);
                auto myObject1 = memBlock.MakeObject<TestObject<2>, size_t>(20);
                {
                    auto myObject2 = memBlock.MakeObject<TestObject<2>, size_t>(30);
                    myObject2->Validate(30, *myObject2);
                }
                myObject1->Validate(20, *myObject1);
            }
            ASSERT_EQ(sizeof(TestObject<4>), memBlock.UsedSize());

            auto myObject1 = memBlock.MakeObject<TestObject<2>, size_t>(40);
            myObject1->Validate(40, *myObject1);
            auto myObject2 = memBlock.MakeObject<TestObject<2>, size_t>(50);
            myObject2->Validate(50, *myObject2);

            myObject->Validate(10, *myObject);

            myObject1.reset();

            auto myObject3 = memBlock.MakeObject<TestObject<8>, size_t>(5);
            myObject3->Validate(5, *myObject3);
            myObject3.reset();

            myObject->Validate(10, *myObject);
            myObject2->Validate(50, *myObject2);
        }
    }
} // namespace nate::Test