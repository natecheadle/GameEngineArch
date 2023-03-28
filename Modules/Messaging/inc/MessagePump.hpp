#pragma once

#include "LargeDataMessage.hpp"
#include "Message.hpp"
#include "MessageSubscribers.hpp"
#include "SmallDataMessage.hpp"

#include <NullMutex.hpp>

#include <future>
#include <map>
#include <mutex>

namespace nate::Modules::Messaging
{
    template <class ID_T, class MUTEX = NullMutex>
    class MessagePump
    {
        std::map<ID_T, MessageSubscribers<ID_T, MUTEX>> m_MessagesSubcriptions;
        MUTEX                                           m_SubscriberMutex;

      public:
        MessagePump() = default;
        ~MessagePump()
        {
            std::unique_lock<MUTEX> lock;
            m_MessagesSubcriptions.clear();
        }

        void PushMessageSync(const Message<ID_T>* pMessage)
        {
            std::unique_lock<MUTEX> lock(m_SubscriberMutex);

            auto it = m_MessagesSubcriptions.find(pMessage->ID());
            if (it == m_MessagesSubcriptions.end())
                return;

            it->second.PushMessage(pMessage);
        }

        std::future<void> PushMessage(Message<ID_T> message)
        {
            return std::async(
                std::launch::async,
                [this](Message<ID_T> message) { PushMessageSync(&message); },
                message);
        }

        template <class DATA>
        std::future<void> PushMessage(LargeDataMessage<ID_T, DATA> message)
        {
            return std::async(std::launch::async, [this, message = std::move(message)] { PushMessageSync(&message); });
        }

        template <class DATA>
        std::future<void> PushMessage(SmallDataMessage<ID_T, DATA> message)
        {
            return std::async(
                std::launch::async,
                [this](SmallDataMessage<ID_T, DATA> message) { PushMessageSync(&message); },
                message);
        }

        bool Subscribe(void* subscriber, ID_T messageID, std::function<void(const Message<ID_T>* msg)> callback)
        {
            std::shared_lock<MUTEX> lock(m_SubscriberMutex);
            auto                    it = m_MessagesSubcriptions.find(messageID);
            if (it == m_MessagesSubcriptions.end())
            {
                lock.unlock();
                std::unique_lock<MUTEX>         uniqueLock(m_SubscriberMutex);
                MessageSubscribers<ID_T, MUTEX> newSubs;
                newSubs.Subscribe(subscriber, std::move(callback));

                m_MessagesSubcriptions.insert({messageID, std::move(newSubs)});
                return true;
            }

            return it->second.Subscribe(subscriber, std::move(callback));
        }

        void Unsubscribe(void* subscriber, ID_T messageID)
        {
            std::unique_lock<MUTEX> lock(m_SubscriberMutex);

            auto it = m_MessagesSubcriptions.find(messageID);
            if (it != m_MessagesSubcriptions.end())
            {
                it->second.Unsubscribe(subscriber);
            }
        }

        void Unsubscribe(void* subscriber)
        {
            std::unique_lock<MUTEX> lock(m_SubscriberMutex);
            for (auto& subscribers : m_MessagesSubcriptions)
            {
                subscribers.second.Unsubscribe(subscriber);
            }
        }

        bool IsSubscribed(void* subscriber, ID_T messageID)
        {
            std::shared_lock<MUTEX> lock(m_SubscriberMutex);

            auto it = m_MessagesSubcriptions.find(messageID);
            if (it != m_MessagesSubcriptions.end())
            {
                return it->second.IsSubscribed(subscriber);
            }

            return false;
        }
    };
} // namespace nate::Modules::Messaging