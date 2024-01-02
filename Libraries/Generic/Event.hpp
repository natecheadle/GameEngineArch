#pragma once

#include "EventSubscription.hpp"
#include "MutexProtected.hpp"

#include <algorithm>
#include <cassert>
#include <future>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace Ignosi::Libraries
{
    template <class... Args>
    class Event
    {
        friend EventSubscription<Args...>;

        MutexProtected<std::mutex, std::vector<EventSubscription<Args...>*>> m_Subscribers;

      public:
        Event() {}

        std::future<void> InvokeAsync(Args... args)
        {

            std::async([&]() { Invoke(std::forward(args...)); });
        }

        void Invoke(Args... args)
        {
            m_Subscribers.execute([&](std::vector<EventSubscription<Args...>*>& subscribers) {
                for (const auto& subscriber : subscribers)
                {
                    subscriber->Invoke(args...);
                }
            });
        }

        void operator()(Args... args) { return Invoke(args...); }

        std::unique_ptr<EventSubscription<Args...>> Subscribe(std::function<void(Args...)> onEvent)
        {
            auto rslt = std::unique_ptr<EventSubscription<Args...>>(new EventSubscription<Args...>(this, std::move(onEvent)));
            m_Subscribers.execute([&](std::vector<EventSubscription<Args...>*>& subscribers) {
                subscribers.push_back(rslt.get());
                std::sort(subscribers.begin(), subscribers.end());
            });

            return rslt;
        }

      private:
        void Unsubscribe(const EventSubscription<Args...>* subscription)
        {
            m_Subscribers.execute([&](std::vector<EventSubscription<Args...>*>& subscribers) {
                auto rslt = std::lower_bound(subscribers.begin(), subscribers.end(), subscription);
                if (rslt != subscribers.end())
                    subscribers.erase(rslt);
                else
                    assert(rslt != subscribers.end());
            });
        }
    };
} // namespace Ignosi::Libraries
