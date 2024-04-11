#include <ConcurrentStack.hpp>
#include <IsStack.h>
#include <LocklessStack.hpp>
#include <Stack.hpp>

#include <gtest/gtest.h>

#include <future>
#include <memory>

using namespace Ignosi::Libraries;

namespace Ignosi::Test
{
    struct StackTestData
    {
        StackTestData()
            : Value1(0)
            , Value2(0)
        {
        }

        StackTestData(int value1, int value2)
            : Value1(value1)
            , Value2(value2)
        {
        }

        int Value1;
        int Value2;

        friend bool operator==(const StackTestData& lhs, const StackTestData& rhs) = default;
    };

    using StackTypes = testing::
        Types<Containers::Stack<StackTestData>, Containers::ConcurrentStack<StackTestData>, Containers::LocklessStack<StackTestData>>;

    template <Containers::IsStack<StackTestData> STACK>
    class StackFixture : public testing::Test
    {
    };

    TYPED_TEST_SUITE(StackFixture, StackTypes);

    TYPED_TEST(StackFixture, ValidateEmpty)
    {
        TypeParam Stack{};

        ASSERT_TRUE(Stack.empty());
        Stack.push({1, 2});
        ASSERT_FALSE(Stack.empty());
        Stack.pop();
        ASSERT_TRUE(Stack.empty());
    }

    TYPED_TEST(StackFixture, ValidatePush)
    {
        TypeParam Stack{};

        Stack.push({5, 1});
        Stack.push({6, 2});
        Stack.push({7, 3});

        ASSERT_EQ(Stack.size(), 3U);
    }

    TYPED_TEST(StackFixture, ValidatePop)
    {
        TypeParam Stack{};

        Stack.push({3, 5});
        Stack.push({2, 4});
        Stack.push({1, 3});

        ASSERT_EQ(Stack.pop(), StackTestData(1, 3));
        ASSERT_EQ(Stack.pop(), StackTestData(2, 4));
        ASSERT_EQ(Stack.pop(), StackTestData(3, 5));
    }

    TYPED_TEST(StackFixture, ValidateMove)
    {
        TypeParam Stack{};

        Stack.push({3, 5});
        Stack.push({2, 4});
        Stack.push({1, 3});

        TypeParam Stack2(std::move(Stack));

        ASSERT_TRUE(Stack.empty());

        ASSERT_EQ(Stack2.pop(), StackTestData(1, 3));
        ASSERT_EQ(Stack2.pop(), StackTestData(2, 4));
        ASSERT_EQ(Stack2.pop(), StackTestData(3, 5));
    }

    TYPED_TEST(StackFixture, ValidateClear)
    {
        TypeParam Stack{};

        Stack.push({3, 5});
        Stack.push({2, 4});
        Stack.push({1, 3});

        ASSERT_FALSE(Stack.empty());

        Stack.clear();

        ASSERT_TRUE(Stack.empty());
    }

    using MTStackTypes = testing::Types<Containers::ConcurrentStack<StackTestData>, Containers::LocklessStack<StackTestData>>;

    template <Containers::IsStack<StackTestData> STACK>
    class MTStackFixture : public testing::Test
    {
    };

    TYPED_TEST_SUITE(MTStackFixture, MTStackTypes);

    TYPED_TEST(MTStackFixture, ValidateMTPush)
    {

        TypeParam Stack{};

        const int itemsPerThreadToPush = 1000;
        const int threadCount          = 10;

        auto pushOntoStack = [&](int id) {
            for (int i = 0; i < itemsPerThreadToPush; ++i)
            {
                Stack.push({id, itemsPerThreadToPush * id + i});
            }
        };

        std::vector<std::future<void>> futures;
        for (int i = 0; i < threadCount; ++i)
        {
            futures.push_back(std::async(pushOntoStack, i));
        }

        for (auto& future : futures)
        {
            future.wait();
        }

        futures.clear();

        ASSERT_EQ(Stack.size(), itemsPerThreadToPush * threadCount);

        auto popOffStack = [&]() {
            for (int i = 0; i < itemsPerThreadToPush; ++i)
            {
                StackTestData rslt;
                ASSERT_NO_THROW(rslt = Stack.pop());
                ASSERT_TRUE(rslt.Value1 >= 0 && rslt.Value1 < threadCount);
                ASSERT_TRUE(
                    rslt.Value2 >= rslt.Value1 * itemsPerThreadToPush &&
                    rslt.Value1 < rslt.Value1 * itemsPerThreadToPush + itemsPerThreadToPush);
            }
        };

        for (int i = 0; i < threadCount; ++i)
        {
            futures.push_back(std::async(popOffStack));
        }

        for (auto& future : futures)
        {
            future.wait();
        }
    }

} // namespace Ignosi::Test
