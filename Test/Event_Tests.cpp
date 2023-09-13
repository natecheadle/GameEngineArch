#include <Event.hpp>
#include <EventSubscriber.hpp>

#include <gtest/gtest.h>

namespace Ignosi::Test
{
    TEST(Event_Tests, ValidateSubscribeUnsubscribeAutomatically)
    {
        Modules::Messaging::Event<> testEvent;
        auto                        subscription = testEvent.Subscribe([]() {});
        ASSERT_TRUE(testEvent.IsSubscribed(subscription));
        ASSERT_NO_THROW(subscription.reset());
    }

    TEST(Event_Tests, ValidateSubscribeUnsubscribeManually)
    {
        Modules::Messaging::Event<> testEvent;
        auto                        subscription = testEvent.Subscribe([]() {});
        ASSERT_TRUE(testEvent.IsSubscribed(subscription));
        ASSERT_TRUE(subscription->IsAlive());
        testEvent.Unsubscribe(subscription.get());
        ASSERT_FALSE(testEvent.IsSubscribed(subscription));
        ASSERT_FALSE(subscription->IsAlive());
    }

    TEST(Event_Tests, ValidateCallbackViaInvoke)
    {
        bool                        callbackReceived{false};
        Modules::Messaging::Event<> testEvent;
        auto                        subscription = testEvent.Subscribe([&]() { callbackReceived = true; });

        testEvent.Invoke();
        ASSERT_TRUE(callbackReceived);
    }

    TEST(Event_Tests, ValidateCallbackViaOperator)
    {
        bool                        callbackReceived{false};
        Modules::Messaging::Event<> testEvent;
        auto                        subscription = testEvent.Subscribe([&]() { callbackReceived = true; });

        testEvent();
        ASSERT_TRUE(callbackReceived);
    }

    TEST(Event_Tests, ValidateCallbackUnsubscribed)
    {
        bool                        callbackReceived{false};
        Modules::Messaging::Event<> testEvent;
        auto                        subscription = testEvent.Subscribe([&]() { callbackReceived = true; });

        testEvent();
        ASSERT_TRUE(callbackReceived);

        callbackReceived = false;
        subscription.reset();
        testEvent();
        ASSERT_FALSE(callbackReceived);
    }

} // namespace Ignosi::Test
