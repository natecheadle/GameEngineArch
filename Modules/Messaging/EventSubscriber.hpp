#pragma once

#include <algorithm>
#include <atomic>
#include <functional>

namespace Ignosi::Modules::Messaging
{
    template <class... Args>
    class Event;

    template <class... Args>
    class EventSubscriber
    {
        friend Event<Args...>;

        std::atomic<Event<Args...>*> m_pParent;
        std::function<void(Args...)> m_OnEvent;

      public:
        EventSubscriber(Event<Args...>* pParent, std::function<void(Args...)> onEvent)
            : m_pParent(pParent)
            , m_OnEvent(std::move(onEvent))
        {
        }

        ~EventSubscriber()
        {
            Event<Args...>* pParent = m_pParent.exchange(nullptr);
            if (pParent)
            {
                pParent->Unsubscribe(this);
            }
        }

        bool IsAlive() const { return m_pParent != nullptr; }

      private:
        void Invoke(Args&&... args) const { m_OnEvent(std::forward(args)...); }
        void OnParentDestroyed() { m_pParent = nullptr; }
    };
} // namespace Ignosi::Modules::Messaging
