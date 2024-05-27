#pragma once

#include <cstddef>
#include <limits>

namespace Ignosi::Libraries::Containers
{
    template <typename T>
    class ObjectPool;

    template <typename T>
    class PoolPointer
    {
        friend ObjectPool<T>;
        static constexpr size_t EMPTY = std::numeric_limits<size_t>::max();

        size_t         m_ID;
        ObjectPool<T>* m_Parent;

        PoolPointer(size_t id, ObjectPool<T>* parent)
            : m_ID(id)
            , m_Parent(parent)
        {
        }

      public:
        PoolPointer()
            : m_ID(EMPTY)
            , m_Parent(nullptr)
        {
        }

        PoolPointer(const PoolPointer&) = delete;

        PoolPointer(PoolPointer&& other) noexcept
            : m_ID(other.m_ID)
            , m_Parent(other.m_Parent)
        {
            other.m_ID     = std::numeric_limits<size_t>::max();
            other.m_Parent = nullptr;
        }

        ~PoolPointer() { Reset(); }

        PoolPointer& operator=(const PoolPointer& other) = delete;

        PoolPointer& operator=(PoolPointer&& other) noexcept
        {
            Reset();

            m_ID     = other.m_ID;
            m_Parent = other.m_Parent;

            other.m_ID     = EMPTY;
            other.m_Parent = nullptr;

            return *this;
        }

        bool IsValid() const { return m_ID != EMPTY; }

        void Reset()
        {
            if (m_ID != EMPTY)
            {
                m_Parent->destroy(m_ID);
                m_ID     = EMPTY;
                m_Parent = nullptr;
            }
        }

        size_t ID() const { return m_ID; }

        T*       Get() { return &(m_Parent->m_Objects[m_ID].Value.value()); }
        const T* Get() const { return &(m_Parent->m_Objects[m_ID].Value.value()); }

        friend bool operator==(const PoolPointer<T>& lhs, const PoolPointer<T>& rhs) = default;

        T* operator->() { return &(m_Parent->m_Objects[m_ID].Value.value()); }
        T& operator*() { return m_Parent->m_Objects[m_ID].Value.value(); }

        const T* operator->() const { return &(m_Parent->m_Objects[m_ID].Value.value()); }
        const T& operator*() const { return m_Parent->m_Objects[m_ID].Value.value(); }
    };

} // namespace Ignosi::Libraries::Containers
