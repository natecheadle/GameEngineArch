#include <ConcurrentQueue.hpp>
#include <IsQueue.h>
#include <LocklessQueue.hpp>
#include <Queue.hpp>

#include <gtest/gtest.h>

using namespace Ignosi::Libraries;

namespace Ignosi::Test
{
    struct QueueTestData
    {
        QueueTestData()
            : Value1(0)
            , Value2(0)
        {
        }

        QueueTestData(int value1, int value2)
            : Value1(value1)
            , Value2(value2)
        {
        }

        int Value1;
        int Value2;

        friend bool operator==(const QueueTestData& lhs, const QueueTestData& rhs) = default;
    };

    using QueueTypes = testing::
        Types<Containers::Queue<QueueTestData>, Containers::ConcurrentQueue<QueueTestData>, Containers::LocklessQueue<QueueTestData>>;

    template <Containers::IsQueue<QueueTestData> STACK>
    class QueueFixture : public testing::Test
    {
    };

    TYPED_TEST_SUITE(QueueFixture, QueueTypes);

    TYPED_TEST(QueueFixture, ValidateEmpty)
    {
        TypeParam queue{};

        ASSERT_TRUE(queue.empty());
        queue.push({1, 2});
        ASSERT_FALSE(queue.empty());
        queue.pop();
        ASSERT_TRUE(queue.empty());
    }

    TYPED_TEST(QueueFixture, ValdiatePushPop)
    {

        TypeParam queue{};
        queue.push({1, 2});
        queue.push({2, 3});
        queue.push({3, 4});

        ASSERT_EQ(QueueTestData(1, 2), queue.pop());
        ASSERT_EQ(QueueTestData(2, 3), queue.pop());
        ASSERT_EQ(QueueTestData(3, 4), queue.pop());
    }

    TYPED_TEST(QueueFixture, ValdiatePushNoPop)
    {

        TypeParam queue{};
        queue.push({1, 2});
        queue.push({2, 3});
        queue.push({3, 4});
    }

    TYPED_TEST(QueueFixture, ValdiateMove)
    {
        TypeParam queue{};
        queue.push({1, 2});
        queue.push({2, 3});
        queue.push({3, 4});

        TypeParam queue2 = std::move(queue);
        ASSERT_TRUE(queue.empty());
        ASSERT_FALSE(queue2.empty());

        ASSERT_EQ(QueueTestData(1, 2), queue2.pop());
        ASSERT_EQ(QueueTestData(2, 3), queue2.pop());
        ASSERT_EQ(QueueTestData(3, 4), queue2.pop());
    }

} // namespace Ignosi::Test
