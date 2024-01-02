#pragma once

#include <functional>

namespace Ignosi::Libraries
{
    template <class... Args>
    class Event;

    template <class... Args>
    class EventSubscription
    {
        friend Event<Args...>;

        Event<Args...>*              m_pParent;
        std::function<void(Args...)> m_OnEvent;

        EventSubscription(Event<Args...>* pParent, std::function<void(Args...)> onEvent) noexcept
            : m_pParent(pParent)
            , m_OnEvent((std::move(onEvent)))
        {
        }

      public:
        EventSubscription(const EventSubscription& other) = delete;
        EventSubscription(EventSubscription&& other)      = delete;

        ~EventSubscription() { m_pParent->Unsubscribe(this); }

      private:
        void Invoke(Args... args) const { m_OnEvent(args...); }
        void operator()(Args... args) const { Invoke(args...); }
    };
} // namespace Ignosi::Libraries

#include "Event.hpp"
