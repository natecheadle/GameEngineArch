#include <Event.hpp>
#include <EventSubscription.hpp>

#include <gtest/gtest.h>

using namespace Ignosi::Libraries;

namespace Ignosi::Test
{
    class EventFixture : public testing::Test
    {
      protected:
        bool                                 m_EventInvoked{false};
        Event<>                              m_Event;
        std::unique_ptr<EventSubscription<>> m_EventSub;

        void SetUp()
        {
            m_EventSub = m_Event.Subscribe([&]() { m_EventInvoked = true; });
        }
    };

    TEST_F(EventFixture, SubscribeSuccessful)
    {
        ASSERT_NE(nullptr, m_EventSub.get());
    }

    TEST_F(EventFixture, InvokeEventViaOperator)
    {
        m_Event();
        ASSERT_TRUE(m_EventInvoked);
    }

    TEST_F(EventFixture, InvokeEventViaInvokeFunction)
    {
        m_Event.Invoke();
        ASSERT_TRUE(m_EventInvoked);
    }

    TEST_F(EventFixture, UnsubscribeSuccessful)
    {
        m_EventSub.reset();
        m_Event.Invoke();
        ASSERT_FALSE(m_EventInvoked);
    }

    TEST_F(EventFixture, MultipleSubscribeSuccessful)
    {
        bool eventInvoked{false};
        auto event_sub2 = m_Event.Subscribe([&]() { eventInvoked = true; });

        m_Event();
        ASSERT_TRUE(m_EventInvoked);
        ASSERT_TRUE(eventInvoked);
    }

    TEST_F(EventFixture, MultipleSubscribeThenUnsuscribeSuccessful)
    {
        bool eventInvoked{false};
        auto event_sub2 = m_Event.Subscribe([&]() { eventInvoked = true; });

        m_EventSub.reset();
        m_Event();
        ASSERT_FALSE(m_EventInvoked);
        ASSERT_TRUE(eventInvoked);
    }

    class EventArgsFixture : public testing::Test
    {
      protected:
        int                                          m_EventArg1{0};
        int                                          m_EventArg2{0};
        Event<int, int>                              m_Event;
        std::unique_ptr<EventSubscription<int, int>> m_EventSub;

        void SetUp()
        {
            m_EventSub = m_Event.Subscribe([&](int first, int second) {
                m_EventArg1 = first;
                m_EventArg2 = second;
            });
        }
    };

    TEST_F(EventArgsFixture, InvokeSuccessful)
    {
        m_Event(10, 20);
        ASSERT_EQ(m_EventArg1, 10);
        ASSERT_EQ(m_EventArg2, 20);
    }

    struct BigEventData
    {
        int First;
        int Second;
        int Third;

        friend bool operator==(const BigEventData& lhs, const BigEventData& rhs) = default;
    };

    class EventBigArgsFixture : public testing::Test
    {
      protected:
        BigEventData                                                                 m_EventArg1{.First = 0, .Second = 0, .Third = 0};
        BigEventData                                                                 m_EventArg2{.First = 0, .Second = 0, .Third = 0};
        Event<const BigEventData&, const BigEventData&>                              m_Event;
        std::unique_ptr<EventSubscription<const BigEventData&, const BigEventData&>> m_EventSub;

        void SetUp()
        {
            m_EventSub = m_Event.Subscribe([&](const BigEventData& first, const BigEventData& second) {
                m_EventArg1 = first;
                m_EventArg2 = second;
            });
        }
    };

    TEST_F(EventBigArgsFixture, InvokeSuccessful)
    {
        BigEventData arg1 = {.First = 10, .Second = -20, .Third = 25};
        BigEventData arg2 = {.First = 30, .Second = -50, .Third = 125};
        m_Event(arg1, arg2);
        ASSERT_EQ(m_EventArg1, arg1);
        ASSERT_EQ(m_EventArg2, arg2);
    }

} // namespace Ignosi::Test
