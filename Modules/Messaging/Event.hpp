#pragma once

#include "EventSubscriber.hpp"

#include <MutexProtected.hpp>

#include <functional>
#include <memory>
#include <vector>

namespace Ignosi::Modules::Messaging
{
    template <class... Args>
    class Event
    {
        MutexProtected<std::mutex, std::vector<EventSubscriber<Args...>*>> m_Subscribers;

      public:
        Event() = default;

        Event(const Event& other) = delete;
        Event(Event&& other)      = default;

        Event& operator=(const Event& other) = delete;
        Event& operator=(Event&& other)      = default;

        std::unique_ptr<EventSubscriber<Args...>> Subscribe(std::function<void(Args...)> onEvent)
        {
            auto rslt = std::make_unique<EventSubscriber<Args...>>(this, std::move(onEvent));
            m_Subscribers.execute([&](std::vector<EventSubscriber<Args...>*>& subscribers) { subscribers.push_back(rslt.get()); });
            return rslt;
        }

        void operator()(Args... args) const { Invoke(args...); }

        void Invoke(Args... args) const
        {
            m_Subscribers.execute([&](const std::vector<EventSubscriber<Args...>*>& subscribers) {
                for (auto& sub : subscribers)
                {
                    sub->Invoke(args...);
                }
            });
        }

        void Unsubscribe(EventSubscriber<Args...>* pSubscriber)
        {
            m_Subscribers.execute([&](std::vector<EventSubscriber<Args...>*>& subscribers) {
                auto it = std::find(subscribers.begin(), subscribers.end(), pSubscriber);
                assert(it != subscribers.end());
                subscribers.erase(it);
            });
            pSubscriber->OnParentDestroyed();
        }

        bool IsSubscribed(EventSubscriber<Args...>* pSubscriber) const
        {
            return m_Subscribers.template execute<bool>([&](const std::vector<EventSubscriber<Args...>*>& subscribers) -> bool {
                return std::find(subscribers.begin(), subscribers.end(), pSubscriber) != subscribers.end();
            });
        }

        bool IsSubscribed(const std::unique_ptr<EventSubscriber<Args...>>& pSubscriber) const { return IsSubscribed(pSubscriber.get()); }
    };
} // namespace Ignosi::Modules::Messaging
