#pragma once

#include "ComponentPointer.h"
#include "IComponent.h"

namespace Ignosi::Modules::ECS
{
    template <ComponentObject T>
    class ComponentPool;

    template <class T>
    class WeakComponentPointer
    {
        ComponentID       m_ID;
        ComponentPool<T>* m_pPool{nullptr};
        friend ComponentPool<T>;

      public:
        WeakComponentPointer() = default;
        WeakComponentPointer(const ComponentPointer<T>& pComponent)
        {
            m_ID    = pComponent.m_ID;
            m_pPool = pComponent.m_pPool;
        }

        WeakComponentPointer(const WeakComponentPointer& other) = default;
        WeakComponentPointer(WeakComponentPointer&& other) noexcept
        {
            m_pPool = other.m_pPool;
            m_ID    = other.m_ID;

            other.m_ID.Reset();
            other.m_pPool = nullptr;
        }

        WeakComponentPointer& operator=(const WeakComponentPointer& other) = default;
        WeakComponentPointer& operator=(WeakComponentPointer&& other) noexcept
        {
            m_pPool = other.m_pPool;
            m_ID    = other.m_ID;

            other.m_ID.Reset();
            other.m_pPool = nullptr;

            return *this;
        }

        ~WeakComponentPointer() = default;

        T*       operator->() { return &(m_pPool->GetComponent(m_ID)); }
        const T* operator->() const { return &(m_pPool->GetComponent(m_ID)); }
        T&       operator*() { return m_pPool->GetComponent(m_ID); }
        const T& operator*() const { return m_pPool->GetComponent(m_ID); }

        void reset()
        {
            m_pPool = nullptr;
            m_ID.Reset();
        }

        T*       get() { return &(m_pPool->GetComponent(m_ID)); }
        const T* get() const { return &(m_pPool->GetComponent(m_ID)); }
    };

} // namespace Ignosi::Modules::ECS

#include "ComponentPool.h"