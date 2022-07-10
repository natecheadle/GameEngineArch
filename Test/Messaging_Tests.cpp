#include <MessagePump.hpp>

#include <gtest/gtest.h>

#include <atomic>
#include <memory>

using namespace nate::Modules;

namespace nate::Test {
    enum class MessageID
    {
        DoA,
        DoB,
        DoC,
    };
    using TestMessage = Messaging::Message<MessageID>;
    using TestPump    = Messaging::MessagePump<MessageID>;

    class MessagePump_Tests : public testing::Test {
      protected:
        TestPump m_MessagePump;

        std::atomic<int> m_DoACallCount{0};
        std::atomic<int> m_DoBCallCount{0};
        std::atomic<int> m_DoCCallCount{0};

        void SetUp()
        {
            ASSERT_TRUE(m_MessagePump.Subscribe(this, MessageID::DoA, [this](const TestMessage*) { DoA(); }));
            ASSERT_TRUE(m_MessagePump.Subscribe(this, MessageID::DoB, [this](const TestMessage*) { DoB(); }));
            ASSERT_TRUE(m_MessagePump.Subscribe(this, MessageID::DoC, [this](const TestMessage*) { DoC(); }));
        }
        void TearDown() {}

        void DoA() { m_DoACallCount++; }
        void DoB() { m_DoBCallCount++; }
        void DoC() { m_DoCCallCount++; }
    };

    TEST_F(MessagePump_Tests, TestDoASync)
    {
        TestMessage doA(MessageID::DoA);
        m_MessagePump.PushMessageSync(&doA);
        ASSERT_EQ(1, m_DoACallCount);
    }

    TEST_F(MessagePump_Tests, TestDoABCSync)
    {
        TestMessage doA(MessageID::DoA);
        TestMessage doB(MessageID::DoB);
        TestMessage doC(MessageID::DoC);

        m_MessagePump.PushMessageSync(&doA);
        m_MessagePump.PushMessageSync(&doB);
        m_MessagePump.PushMessageSync(&doC);

        ASSERT_EQ(1, m_DoACallCount);
        ASSERT_EQ(1, m_DoBCallCount);
        ASSERT_EQ(1, m_DoCCallCount);
    }

    TEST_F(MessagePump_Tests, TestDoABCASync)
    {
        std::future<void> aFut = m_MessagePump.PushMessage(std::make_unique<TestMessage>(MessageID::DoA));
        std::future<void> bFut = m_MessagePump.PushMessage(std::make_unique<TestMessage>(MessageID::DoB));
        std::future<void> cFut = m_MessagePump.PushMessage(std::make_unique<TestMessage>(MessageID::DoC));

        aFut.wait();
        bFut.wait();
        cFut.wait();

        ASSERT_EQ(1, m_DoACallCount);
        ASSERT_EQ(1, m_DoBCallCount);
        ASSERT_EQ(1, m_DoCCallCount);
    }

    TEST_F(MessagePump_Tests, TestDoABCManyASync)
    {
        std::vector<std::future<void>> futures(30);
        for (size_t i = 0; i < 10; ++i)
        {
            size_t initital       = i * 3;
            futures[initital]     = std::move(m_MessagePump.PushMessage(std::make_unique<TestMessage>(MessageID::DoA)));
            futures[initital + 1] = std::move(m_MessagePump.PushMessage(std::make_unique<TestMessage>(MessageID::DoB)));
            futures[initital + 2] = std::move(m_MessagePump.PushMessage(std::make_unique<TestMessage>(MessageID::DoC)));
        }

        for (auto& future : futures)
        {
            future.wait();
        }

        ASSERT_EQ(10, m_DoACallCount);
        ASSERT_EQ(10, m_DoBCallCount);
        ASSERT_EQ(10, m_DoCCallCount);
    }
} // namespace nate::Test