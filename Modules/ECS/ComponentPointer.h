#pragma once

#include "ComponentID.h"

namespace Ignosi::Modules::ECS
{
    template <class T>
    class ComponentPool;

    template <class T>
    class ComponentPointer
    {
        ComponentID       m_ID{ComponentID::RESET_VAL};
        ComponentPool<T>* m_pPool{nullptr};
        friend ComponentPool<T>;

        ComponentPointer(ComponentID id, ComponentPool<T>* pPool)
            : m_ID(id)
            , m_pPool(pPool)
        {
        }

      public:
        ComponentPointer() = default;

        ComponentPointer(const ComponentPointer& other) = delete;
        ComponentPointer(ComponentPointer&& other) noexcept
        {
            m_pPool = other.m_pPool;
            m_ID    = other.m_ID;

            other.m_ID.Reset();
            other.m_pPool = nullptr;
        }

        ComponentPointer& operator=(const ComponentPointer& other) = delete;
        ComponentPointer& operator=(ComponentPointer&& other) noexcept
        {
            m_pPool = other.m_pPool;
            m_ID    = other.m_ID;

            other.m_ID.Reset();
            other.m_pPool = nullptr;

            return *this;
        }

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