#include <DynamicRingBuffer.hpp>
#include <IsRingBuffer.h>
#include <RingBuffer.hpp>

#include <gtest/gtest.h>

using namespace Ignosi::Libraries;

namespace Ignosi::Test
{
    namespace
    {
        struct Data
        {
            size_t      Value1;
            size_t      Value2;
            friend bool operator==(const Data& lhs, const Data& rhs) = default;
        };
        static constexpr size_t BufferSize = 64;

    } // namespace

    using BufferTypes = testing::Types<Containers::RingBuffer<Data, BufferSize>, Containers::DynamicRingBuffer<Data>>;

    template <Containers::IsRingBuffer<Data> BUFFER>
    class RingBufferFixture : public testing::Test
    {
    };

    TYPED_TEST_SUITE(RingBufferFixture, BufferTypes);

    TYPED_TEST(RingBufferFixture, ValidateFullEmpty)
    {
        TypeParam buffer{};

        ASSERT_FALSE(buffer.full());
        ASSERT_TRUE(buffer.empty());

        for (size_t i = 0; i < BufferSize; ++i)
        {
            ASSERT_TRUE(buffer.push({i, i + 1}));
        }

        ASSERT_FALSE(buffer.empty());
        ASSERT_EQ(buffer.count(), BufferSize);

        for (size_t i = 0; i < 10; ++i)
        {

            ASSERT_TRUE(buffer.pop());
        }

        ASSERT_FALSE(buffer.empty());

        for (size_t i = 0; i < 10; ++i)
        {

            ASSERT_TRUE(buffer.push({64 + i, 65 + i}));
        }

        ASSERT_EQ(buffer.count(), BufferSize);
    }

    TYPED_TEST(RingBufferFixture, ValidatePushPop)
    {
        TypeParam buffer{};

        for (size_t i = 0; i < BufferSize; ++i)
        {
            Data toPush = {i, i + 1};
            ASSERT_EQ(toPush, buffer.push(toPush));
        }

        for (size_t i = 0; i < BufferSize; ++i)
        {

            ASSERT_EQ(Data({i, i + 1}), buffer.pop());
        }
    }
} // namespace Ignosi::Test
