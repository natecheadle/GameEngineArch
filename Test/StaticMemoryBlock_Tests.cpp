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

        ~TestObject() { std::memset(&m_Value[0], 0x00, sizeof(m_Value)); }

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

    TEST(StaticMemoryBlock_Tests, ValidateLinearMemoryBlockBasicCreateDelete)
    {
        Modules::Memory::StaticLinearMemoryBlock<MemoryBuffer, MemorySize> memBlock;

        auto myObject = memBlock.MakeObject<TestObject<2>, size_t>(10);
        myObject->Validate(10, *myObject);
        auto myObject1 = memBlock.MakeObject<TestObject<4>, size_t>(20);
        myObject1->Validate(20, *myObject1);
        auto myObject2 = memBlock.MakeObject<TestObject<8>, size_t>(30);
        myObject2->Validate(30, *myObject2);

        ASSERT_EQ(sizeof(TestObject<2>) + sizeof(TestObject<4>) + sizeof(TestObject<8>), memBlock.UsedSize());

        memBlock.Reset();
        ASSERT_EQ(0, memBlock.UsedSize());

        auto myObject4 = memBlock.MakeObject<TestObject<1024>, size_t>(1);
        ASSERT_EQ(nullptr, myObject4);
    }

    TEST(StaticMemoryBlock_Tests, ValidateLinearMemoryExceedSize)
    {
        Modules::Memory::StaticLinearMemoryBlock<MemoryBuffer, MemorySize> memBlock;

        auto myObject = memBlock.MakeObject<TestObject<1024>, size_t>(1);
        ASSERT_EQ(nullptr, myObject);
    }

    TEST(StaticMemoryBlock_Tests, ValidateStackMemoryBlockBasicCreateDelete)
    {
        Modules::Memory::StaticStackMemoryBlock<MemoryBuffer, MemorySize> memBlock;
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
    }

    TEST(StaticMemoryBlock_Tests, ValidateStackMemoryExceedSize)
    {
        Modules::Memory::StaticStackMemoryBlock<MemoryBuffer, MemorySize> memBlock;

        auto myObject = memBlock.MakeObject<TestObject<1024>, size_t>(1);
        ASSERT_EQ(nullptr, myObject);
    }

    TEST(StaticMemoryBlock_Tests, ValidatePoolMemoryBlockBasicCreateDelete)
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

    TEST(StaticMemoryBlock_Tests, ValidatePoolMemoryExceedSize)
    {
        Modules::Memory::StaticPoolMemoryBlock<TestObject<2>, MemoryBuffer, MemorySize> memBlock;

        std::vector<std::unique_ptr<TestObject<2>, std::function<void(TestObject<2>*)>>> objects;

        static constexpr size_t maxObjects = MemorySize / sizeof(TestObject<2>);

        objects.reserve(maxObjects);

        for (size_t i = 0; i < maxObjects; ++i)
        {
            auto myObject = memBlock.MakeObject<size_t>(10 * i);
            myObject->Validate(10 * i, *myObject);
            ASSERT_EQ((i + 1) * sizeof(TestObject<2>), memBlock.UsedSize());

            objects.push_back(std::move(myObject));
        }

        auto myObject = memBlock.MakeObject<size_t>(10);
        ASSERT_EQ(nullptr, myObject);

        ASSERT_NO_THROW(objects.clear());

        ASSERT_EQ(0, memBlock.UsedSize());
    }

    TEST(StaticMemoryBlock_Tests, ValidateFreeMemoryBlockBasicCreateDelete)
    {
        Modules::Memory::StaticFreeMemoryBlock<MemoryBuffer, MemorySize> memBlock;
        {
            auto myObject = memBlock.MakeObject<TestObject<4>, size_t>(10);
            myObject->Validate(10, *myObject);
            {
                auto myObject1 = memBlock.MakeObject<TestObject<2>, size_t>(20);
                myObject1->Validate(20, *myObject1);
                ASSERT_EQ(sizeof(TestObject<4>) + sizeof(TestObject<2>), memBlock.UsedSize());
                {
                    auto myObject2 = memBlock.MakeObject<TestObject<2>, size_t>(30);
                    myObject2->Validate(30, *myObject2);

                    ASSERT_EQ(sizeof(TestObject<4>) + sizeof(TestObject<2>) * 2, memBlock.UsedSize());
                }
                myObject1->Validate(20, *myObject1);
            }
            myObject->Validate(10, *myObject);

            ASSERT_EQ(sizeof(TestObject<4>), memBlock.UsedSize());
        }

        ASSERT_EQ(0, memBlock.UsedSize());
    }

    TEST(StaticMemoryBlock_Tests, ValidateFreeMemoryBlockDeleteOutOfOrder)
    {
        Modules::Memory::StaticFreeMemoryBlock<MemoryBuffer, MemorySize> memBlock;

        auto myObject = memBlock.MakeObject<TestObject<4>, size_t>(10);
        myObject->Validate(10, *myObject);
        ASSERT_EQ(sizeof(TestObject<4>), memBlock.UsedSize());

        auto myObject1 = memBlock.MakeObject<TestObject<2>, size_t>(20);
        myObject1->Validate(20, *myObject1);
        ASSERT_EQ(sizeof(TestObject<4>) + sizeof(TestObject<2>), memBlock.UsedSize());

        auto myObject2 = memBlock.MakeObject<TestObject<2>, size_t>(30);
        myObject2->Validate(30, *myObject2);
        ASSERT_EQ(sizeof(TestObject<4>) + sizeof(TestObject<2>) * 2, memBlock.UsedSize());

        myObject1.reset();
        myObject.reset();
        myObject2.reset();

        ASSERT_EQ(0, memBlock.UsedSize());
    }

    TEST(StaticMemoryBlock_Tests, ValidateFreeMemoryBlockCreateObjectFragmentedMemory)
    {
        Modules::Memory::StaticFreeMemoryBlock<MemoryBuffer, MemorySize> memBlock;

        auto myObject1 = memBlock.MakeObject<TestObject<2>, size_t>(20);
        myObject1->Validate(20, *myObject1);
        ASSERT_EQ(sizeof(TestObject<2>), memBlock.UsedSize());

        auto myObject2 = memBlock.MakeObject<TestObject<2>, size_t>(30);
        myObject2->Validate(30, *myObject2);
        ASSERT_EQ(sizeof(TestObject<2>) * 2, memBlock.UsedSize());

        myObject1.reset();

        auto myObject = memBlock.MakeObject<TestObject<4>, size_t>(10);
        myObject->Validate(10, *myObject);
        ASSERT_EQ(sizeof(TestObject<4>) + sizeof(TestObject<2>), memBlock.UsedSize());

        myObject2->Validate(30, *myObject2);
    }

    TEST(StaticMemoryBlock_Tests, ValidateFreeMemoryBlockCreateObjectVeryFragmentedMemory)
    {
        Modules::Memory::StaticFreeMemoryBlock<MemoryBuffer, MemorySize> memBlock;

        auto myObject1 = memBlock.MakeObject<TestObject<2>, size_t>(20);
        myObject1->Validate(20, *myObject1);
        ASSERT_EQ(sizeof(TestObject<2>), memBlock.UsedSize());

        auto myObject2 = memBlock.MakeObject<TestObject<2>, size_t>(30);
        myObject2->Validate(30, *myObject2);
        ASSERT_EQ(sizeof(TestObject<2>) * 2, memBlock.UsedSize());

        auto myObject3 = memBlock.MakeObject<TestObject<2>, size_t>(40);
        myObject3->Validate(40, *myObject3);
        ASSERT_EQ(sizeof(TestObject<2>) * 3, memBlock.UsedSize());

        auto myObject4 = memBlock.MakeObject<TestObject<2>, size_t>(50);
        myObject4->Validate(50, *myObject4);
        ASSERT_EQ(sizeof(TestObject<2>) * 4, memBlock.UsedSize());

        myObject1.reset();
        myObject3.reset();

        auto myObject = memBlock.MakeObject<TestObject<4>, size_t>(10);
        myObject->Validate(10, *myObject);
        ASSERT_EQ(sizeof(TestObject<4>) + sizeof(TestObject<2>) * 2, memBlock.UsedSize());

        myObject2->Validate(30, *myObject2);
        myObject4->Validate(50, *myObject4);
    }

    TEST(StaticMemoryBlock_Tests, ValidateFreeMemoryBlockCreateObjectSameSizeFragmentedMemory)
    {
        Modules::Memory::StaticFreeMemoryBlock<MemoryBuffer, MemorySize> memBlock;

        auto myObject1 = memBlock.MakeObject<TestObject<2>, size_t>(20);
        myObject1->Validate(20, *myObject1);
        ASSERT_EQ(sizeof(TestObject<2>), memBlock.UsedSize());

        auto myObject2 = memBlock.MakeObject<TestObject<2>, size_t>(30);
        myObject2->Validate(30, *myObject2);
        ASSERT_EQ(sizeof(TestObject<2>) * 2, memBlock.UsedSize());

        auto myObject3 = memBlock.MakeObject<TestObject<2>, size_t>(40);
        myObject3->Validate(40, *myObject3);
        ASSERT_EQ(sizeof(TestObject<2>) * 3, memBlock.UsedSize());

        auto myObject4 = memBlock.MakeObject<TestObject<2>, size_t>(50);
        myObject4->Validate(50, *myObject4);
        ASSERT_EQ(sizeof(TestObject<2>) * 4, memBlock.UsedSize());

        myObject1.reset();
        myObject3.reset();

        auto myObject = memBlock.MakeObject<TestObject<2>, size_t>(10);
        myObject->Validate(10, *myObject);
        ASSERT_EQ(sizeof(TestObject<2>) * 3, memBlock.UsedSize());

        myObject2->Validate(30, *myObject2);
        myObject4->Validate(50, *myObject4);
    }

    TEST(StaticMemoryBlock_Tests, ValidateFreeMemoryBlockExceedSize)
    {
        Modules::Memory::StaticFreeMemoryBlock<MemoryBuffer, MemorySize> memBlock;

        auto myObject = memBlock.MakeObject<TestObject<1024>, size_t>(1);
        ASSERT_EQ(nullptr, myObject);
    }
} // namespace nate::Test