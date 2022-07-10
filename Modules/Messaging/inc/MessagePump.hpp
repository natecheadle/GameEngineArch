#pragma once

#include "Message.hpp"
#include "MessageSubscribers.hpp"

#include <future>
#include <map>
#include <mutex>
#include <shared_mutex>

namespace nate::Modules::Messaging {
    template <class ID_T>
    class MessagePump {
        std::map<ID_T, MessageSubscribers<ID_T>> m_MessagesSubcriptions;
        std::shared_mutex                        m_SubscriberMutex;

      public:
        MessagePump() = default;
        ~MessagePump()
        {
            std::unique_lock<std::shared_mutex> lock;
            m_MessagesSubcriptions.clear();
        }

        void PushMessageSync(const Message<ID_T>* pMessage)
        {
            std::shared_lock<std::shared_mutex> lock(m_SubscriberMutex);

            auto it = m_MessagesSubcriptions.find(pMessage->ID());
            if (it == m_MessagesSubcriptions.end())
                return;

            it->second.PushMessage(pMessage);
        }

        std::future<void> PushMessage(std::unique_ptr<Message<ID_T>> pMessage)
        {
            return std::async(std::launch::async, &MessagePump<ID_T>::PushMessageUniqueSync, this, std::move(pMessage));
        }

        bool Subscribe(void* subscriber, ID_T messageID, std::function<void(const Message<ID_T>* msg)> callback)
        {
            std::shared_lock<std::shared_mutex> lock(m_SubscriberMutex);
            auto                                it = m_MessagesSubcriptions.find(messageID);
            if (it == m_MessagesSubcriptions.end())
            {
                lock.unlock();
                std::unique_lock<std::shared_mutex> uniqueLock(m_SubscriberMutex);
                MessageSubscribers<ID_T>            newSubs;
                newSubs.Subscribe(subscriber, std::move(callback));

                m_MessagesSubcriptions.insert({messageID, std::move(newSubs)});
                return true;
            }

            return it->second.Subscribe(subscriber, std::move(callback));
        }

        void Unsubscribe(void* subscriber, ID_T messageID)
        {
            std::shared_lock<std::shared_mutex> lock(m_SubscriberMutex);

            auto it = m_MessagesSubcriptions.find(messageID);
            if (it != m_MessagesSubcriptions.end())
            {
                it->second.Unsubscribe(subscriber);
            }
        }
        bool IsSubscribed(void* subscriber, ID_T messageID)
        {
            std::shared_lock<std::shared_mutex> lock(m_SubscriberMutex);

            auto it = m_MessagesSubcriptions.find(messageID);
            if (it != m_MessagesSubcriptions.end())
            {
                return it->second.IsSubscribed(subscriber);
            }

            return false;
        }

      private:
        void PushMessageUniqueSync(std::unique_ptr<Message<ID_T>> pMessage) { PushMessageSync(pMessage.get()); }
    };
} // namespace nate::Modules::Messaging