#pragma once

#include "Message.hpp"

#include <map>
#include <mutex>
#include <shared_mutex>
#include <functional>

namespace nate::Modules::Messaging {
    template <class ID_T>
    class MessageSubscribers {
        std::map<void*, std::function<void(const Message<ID_T>* msg)>> m_Subscribers;
        std::shared_mutex                                              m_SubscriberMutex;

      public:
        MessageSubscribers() = default;
        ~MessageSubscribers()
        {
            std::unique_lock<std::shared_mutex> lock;
            m_Subscribers.clear();
        }

        MessageSubscribers(const MessageSubscribers& other)            = delete;
        MessageSubscribers& operator=(const MessageSubscribers& other) = delete;

        MessageSubscribers(MessageSubscribers&& other)
            : m_Subscribers(std::move(other.m_Subscribers))
        {
        }
        MessageSubscribers& operator=(MessageSubscribers&& other)
        {
            m_Subscribers = std::move(other.m_Subscribers);
            return *this;
        }

        bool Subscribe(void* subscriber, std::function<void(const Message<ID_T>* msg)> callback)
        {
            std::unique_lock<std::shared_mutex> lock(m_SubscriberMutex);

            auto it = m_Subscribers.find(subscriber);
            if (it != m_Subscribers.end())
            {
                return false;
            }

            m_Subscribers.insert({subscriber, std::move(callback)});
            return true;
        }

        void Unsubscribe(void* subscriber)
        {
            std::unique_lock<std::shared_mutex> lock(m_SubscriberMutex);
            m_Subscribers.erase(subscriber);
        }

        bool IsSubscribed(void* subscriber)
        {
            std::shared_lock<std::shared_mutex> lock(m_SubscriberMutex);
            return m_Subscribers.find(subscriber) != m_Subscribers.end();
        }

        void PushMessage(const Message<ID_T>* msg)
        {
            std::shared_lock<std::shared_mutex> lock(m_SubscriberMutex);
            for (auto& subscriber : m_Subscribers)
            {
                subscriber.second(msg);
            }
        }
    };
} // namespace nate::Modules::Messaging