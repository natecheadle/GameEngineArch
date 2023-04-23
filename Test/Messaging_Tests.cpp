#include <MessagePump.hpp>

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <memory>
#include <thread>

using namespace nate::Modules;

namespace nate::Test
{
    enum class MessageID
    {
        DoA,
        DoB,
        DoC,

        DoLongA,
        DoLongB,
        DoLongC,

        Data,
        SmallData,
    };

    struct TestData
    {
        TestData() = default;
        TestData(int item1, int item2, int item3)
            : TestItem1(item1)
            , TestItem2(item2)
            , TestItem3(item3)
        {
        }

        int TestItem1{0};
        int TestItem2{0};
        int TestItem3{0};
    };

    using TestMessage          = Messaging::Message<MessageID>;
    using TestPump_ST          = Messaging::MessagePump<MessageID>;
    using TestPump_MT          = Messaging::MessagePump<MessageID, std::mutex>;
    using TestLargeDataMessage = Messaging::LargeDataMessage<MessageID, TestData>;
    using TestSmallDataMessage = Messaging::SmallDataMessage<MessageID, std::int64_t>;

    class MessagePump_Tests : public testing::Test
    {
      protected:
        TestPump_MT m_MessagePump;

        std::atomic<int> m_DoACallCount{0};
        std::atomic<int> m_DoBCallCount{0};
        std::atomic<int> m_DoCCallCount{0};

        std::atomic<int> m_DoLongACallCount{0};
        std::atomic<int> m_DoLongBCallCount{0};
        std::atomic<int> m_DoLongCCallCount{0};

        std::atomic<int> m_DoDataCount{0};
        std::atomic<int> m_DoSmallDataCount{0};

        void SetUp()
        {
            EXPECT_TRUE(m_MessagePump.Subscribe(this, MessageID::DoA, [this](const TestMessage*) { DoA(); }));
            EXPECT_TRUE(m_MessagePump.Subscribe(this, MessageID::DoB, [this](const TestMessage*) { DoB(); }));
            EXPECT_TRUE(m_MessagePump.Subscribe(this, MessageID::DoC, [this](const TestMessage*) { DoC(); }));

            EXPECT_TRUE(m_MessagePump.Subscribe(this, MessageID::DoLongA, [this](const TestMessage*) { DoLongA(); }));
            EXPECT_TRUE(m_MessagePump.Subscribe(this, MessageID::DoLongB, [this](const TestMessage*) { DoLongB(); }));
            EXPECT_TRUE(m_MessagePump.Subscribe(this, MessageID::DoLongC, [this](const TestMessage*) { DoLongC(); }));

            EXPECT_TRUE(m_MessagePump.Subscribe(this, MessageID::Data, [this](const TestMessage* pMessage) {
                DoData(pMessage);
            }));

            EXPECT_TRUE(m_MessagePump.Subscribe(this, MessageID::SmallData, [this](const TestMessage* pMessage) {
                DoSmallData(pMessage);
            }));
        }
        void TearDown() {}

        void DoA() { m_DoACallCount++; }
        void DoB() { m_DoBCallCount++; }
        void DoC() { m_DoCCallCount++; }

        void DoLongA()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            m_DoLongACallCount++;
        }
        void DoLongB()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            m_DoLongBCallCount++;
        }
        void DoLongC()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            m_DoLongCCallCount++;
        }

        void DoData(const TestMessage* pMessage)
        {
            const auto* pDataMessage = dynamic_cast<const TestLargeDataMessage*>(pMessage);
            if (pDataMessage)
            {
                m_DoDataCount++;
            }
        }

        void DoSmallData(const TestMessage* pMessage)
        {
            const auto* pDataMessage = dynamic_cast<const TestSmallDataMessage*>(pMessage);
            if (pDataMessage)
            {
                m_DoSmallDataCount++;
            }
        }
    };

    TEST_F(MessagePump_Tests, TestDoASync)
    {
        TestMessage doA(MessageID::DoA);
        m_MessagePump.PushMessageSync(&doA);
        EXPECT_EQ(1, m_DoACallCount);
    }

    TEST_F(MessagePump_Tests, TestDoABCSync)
    {
        TestMessage doA(MessageID::DoA);
        TestMessage doB(MessageID::DoB);
        TestMessage doC(MessageID::DoC);

        m_MessagePump.PushMessageSync(&doA);
        m_MessagePump.PushMessageSync(&doB);
        m_MessagePump.PushMessageSync(&doC);

        EXPECT_EQ(1, m_DoACallCount);
        EXPECT_EQ(1, m_DoBCallCount);
        EXPECT_EQ(1, m_DoCCallCount);
    }

    TEST_F(MessagePump_Tests, TestDoABCASync)
    {
        std::future<void> aFut = m_MessagePump.PushMessage(TestMessage(MessageID::DoA));
        std::future<void> bFut = m_MessagePump.PushMessage(TestMessage(MessageID::DoB));
        std::future<void> cFut = m_MessagePump.PushMessage(TestMessage(MessageID::DoC));

        aFut.wait();
        bFut.wait();
        cFut.wait();

        EXPECT_EQ(1, m_DoACallCount);
        EXPECT_EQ(1, m_DoBCallCount);
        EXPECT_EQ(1, m_DoCCallCount);
    }

    TEST_F(MessagePump_Tests, TestDoABCManyASync)
    {
        std::vector<std::future<void>> futures(30);
        for (size_t i = 0; i < 10; ++i)
        {
            size_t initital       = i * 3;
            futures[initital]     = std::move(m_MessagePump.PushMessage(TestMessage(MessageID::DoA)));
            futures[initital + 1] = std::move(m_MessagePump.PushMessage(TestMessage(MessageID::DoB)));
            futures[initital + 2] = std::move(m_MessagePump.PushMessage(TestMessage(MessageID::DoC)));
        }

        for (auto& future : futures)
        {
            future.wait();
        }

        EXPECT_EQ(10, m_DoACallCount);
        EXPECT_EQ(10, m_DoBCallCount);
        EXPECT_EQ(10, m_DoCCallCount);
    }

    TEST_F(MessagePump_Tests, TestDoABCLongManyASync)
    {
        std::vector<std::future<void>> futures(30);
        for (size_t i = 0; i < 10; ++i)
        {
            size_t initital       = i * 3;
            futures[initital]     = std::move(m_MessagePump.PushMessage(TestMessage(MessageID::DoLongA)));
            futures[initital + 1] = std::move(m_MessagePump.PushMessage(TestMessage(MessageID::DoLongB)));
            futures[initital + 2] = std::move(m_MessagePump.PushMessage(TestMessage(MessageID::DoLongC)));
        }

        for (auto& future : futures)
        {
            future.wait();
        }

        EXPECT_EQ(10, m_DoLongACallCount);
        EXPECT_EQ(10, m_DoLongBCallCount);
        EXPECT_EQ(10, m_DoLongCCallCount);
    }

    TEST_F(MessagePump_Tests, ValidateUnsubscribeSingle)
    {
        m_MessagePump.Unsubscribe(this, MessageID::DoA);
        EXPECT_FALSE(m_MessagePump.IsSubscribed(this, MessageID::DoA));

        EXPECT_TRUE(m_MessagePump.IsSubscribed(this, MessageID::DoB));
        EXPECT_TRUE(m_MessagePump.IsSubscribed(this, MessageID::DoC));
        EXPECT_TRUE(m_MessagePump.IsSubscribed(this, MessageID::DoLongA));
        EXPECT_TRUE(m_MessagePump.IsSubscribed(this, MessageID::DoLongB));
        EXPECT_TRUE(m_MessagePump.IsSubscribed(this, MessageID::DoLongC));
    }

    TEST_F(MessagePump_Tests, ValidateAsyncDataMessages)
    {
        std::future<void> largeFut =
            m_MessagePump.PushMessage(TestLargeDataMessage(MessageID::Data, std::make_unique<TestData>(10, 20, 30)));
        std::future<void> smallFut = m_MessagePump.PushMessage(TestSmallDataMessage(MessageID::SmallData, 10));

        largeFut.wait();
        smallFut.wait();

        EXPECT_EQ(1, m_DoSmallDataCount);
        EXPECT_EQ(1, m_DoDataCount);
    }

    TEST_F(MessagePump_Tests, ValidateDataMessages)
    {
        TestLargeDataMessage largeMsg(MessageID::Data, std::make_unique<TestData>(10, 20, 30));
        TestSmallDataMessage smallMsg(MessageID::SmallData, 10);
        m_MessagePump.PushMessageSync(&largeMsg);
        m_MessagePump.PushMessageSync(&smallMsg);

        EXPECT_EQ(1, m_DoSmallDataCount);
        EXPECT_EQ(1, m_DoDataCount);
    }
} // namespace nate::Test