#pragma once

#include "DynamicRingBuffer.hpp"

#include <cstddef>
#include <limits>
#include <utility>
#include <vector>

namespace Ignosi::Libraries::Containers
{
    template <class T>
    class ObjectPool;

    template <class T>
    class PoolPointer
    {
        friend ObjectPool<T>;

        size_t         m_ID;
        ObjectPool<T>& m_Parent;

        PoolPointer(size_t id, ObjectPool<T>& parent)
            : m_ID(id)
            , m_Parent(parent)
        {
        }

      public:
        PoolPointer(const PoolPointer&) = delete;

        PoolPointer(PoolPointer&& other)
            : m_ID(other.m_ID)
            , m_Parent(other.m_Parent)
        {
            other.m_ID = std::numeric_limits<size_t>::max();
        }

        ~PoolPointer() { reset(); }

        PoolPointer& operator=(const PoolPointer& other) = delete;

        PoolPointer& operator=(PoolPointer&& other)
        {
            m_ID       = other.m_ID;
            m_Parent   = other.m_Parent;
            other.m_ID = std::numeric_limits<size_t>::max();
            return *this;
        }

        void reset()
        {
            if (m_ID != std::numeric_limits<size_t>::max())
            {
                m_Parent.QueueDestroy(m_ID);
                m_ID = std::numeric_limits<size_t>::max();
            }
        }

        T* operator->() { return &(m_Parent.m_Objects[m_ID].value()); }
        T& operator*() { return m_Parent.m_Objects[m_ID].value(); }
    };

    template <class T>
    class ObjectPool
    {
        friend PoolPointer<T>;

        std::vector<std::optional<T>> m_Objects;
        DynamicRingBuffer<size_t>     m_FreedObjects;
        DynamicRingBuffer<size_t>     m_ToFreeObjects;

      public:
        template <class... Args>
        PoolPointer<T> Create(T&& obj)
        {
            if (m_FreedObjects.empty())
            {
                m_Objects.emplace_back(std::forward<T>(obj));
                size_t newObj = m_Objects.size() - 1;
                return PoolPointer<T>(newObj, *this);
            }
            else
            {
                size_t newObjLocation = m_FreedObjects.pop().value();
                m_Objects[newObjLocation].emplace(std::forward<T>(obj));
                return PoolPointer<T>(newObjLocation, *this);
            }
        }

        template <class... Args>
        PoolPointer<T> Create(Args... args)
        {
            return Create(T(std::forward<Args>(args)...));
        }

        PoolPointer<T> Create(const T& value) { return Create(T(value)); }

        void Process()
        {
            while (!m_ToFreeObjects.empty())
            {
                size_t objToFree = m_ToFreeObjects.pop().value();
                m_Objects[objToFree].reset();
                m_FreedObjects.push(objToFree);
            }
        }

      private:
        void QueueDestroy(size_t id) { m_ToFreeObjects.push(id); }
    };
} // namespace Ignosi::Libraries::Containers
