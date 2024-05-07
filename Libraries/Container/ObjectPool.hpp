#pragma once

#include "PoolPointer.hpp"

#include <cassert>
#include <cstddef>
#include <iterator>
#include <optional>
#include <utility>
#include <vector>

namespace Ignosi::Libraries::Containers
{

    template <typename T>
    class ObjectPool
    {
        static constexpr size_t DEFAULT_SIZE_INCREASE = 256;
        friend PoolPointer<T>;

        struct Data
        {
            size_t           Next;
            std::optional<T> Value;
        };

        std::vector<Data> m_Objects;
        size_t            m_FirstFree;
        size_t            m_FirstOccupied;

      public:
        ObjectPool()
        {
            m_Objects.resize(DEFAULT_SIZE_INCREASE);

            m_FirstFree     = 0;
            m_FirstOccupied = m_Objects.size();

            m_Objects[0].Next = 1;

            for (size_t i = 1; i < m_Objects.size(); ++i)
            {
                m_Objects[i].Next = i + 1;
            }
        }

        class iterator
        {
            ObjectPool<T>* m_Parent;
            size_t         m_Current;

            iterator(ObjectPool<T>* parent, size_t current)
                : m_Parent(parent)
                , m_Current(current)
            {
            }

            friend ObjectPool<T>;

          public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = T;
            using pointer           = value_type*;
            using reference         = value_type&;

            iterator() {}

            iterator(const iterator& other)     = default;
            iterator(iterator&& other) noexcept = default;

            iterator& operator=(const iterator& other)     = default;
            iterator& operator=(iterator&& other) noexcept = default;

            reference operator*() { return m_Parent->m_Objects[m_Current].Value.value(); }
            pointer   operator->() { return &(m_Parent->m_Objects[m_Current].Value.value()); }

            iterator& operator++()
            {
                m_Current = m_Parent->m_Objects[m_Current].Next;
                return *this;
            }

            iterator operator++(int)
            {
                auto copy = *this;
                m_Current = m_Parent->m_Objects[m_Current].Next;

                return copy;
            }

            friend bool operator==(const iterator& lhs, const iterator& rhs) = default;
            friend bool operator!=(const iterator& lhs, const iterator& rhs) = default;
        };

        class const_iterator
        {
            const ObjectPool<T>* m_Parent;
            size_t               m_Current;

            const_iterator(const ObjectPool<T>* parent, size_t current)
                : m_Parent(parent)
                , m_Current(current)
            {
            }

            friend ObjectPool<T>;

          public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = T;
            using pointer           = const value_type*;
            using reference         = const value_type&;

            const_iterator() {}

            const_iterator(const const_iterator& other)     = default;
            const_iterator(const_iterator&& other) noexcept = default;

            const_iterator(const iterator& other)
                : m_Parent(other.m_Parent)
                , m_Current(other.m_Current)
            {
            }

            const_iterator(iterator&& other) noexcept
                : m_Parent(other.m_Parent)
                , m_Current(other.m_Current)
            {
            }

            const_iterator& operator=(const const_iterator& other)     = default;
            const_iterator& operator=(const_iterator&& other) noexcept = default;

            const_iterator& operator=(const iterator& other)
            {
                m_Parent  = other.m_Parent;
                m_Current = other.m_Current;
                return *this;
            }

            const_iterator& operator=(iterator&& other) noexcept
            {
                m_Parent  = other.m_Parent;
                m_Current = other.m_Current;
                return *this;
            }

            reference operator*() { return m_Parent->m_Objects[m_Current].Value.value(); }
            pointer   operator->() { return &(m_Parent->m_Objects[m_Current].Value.value()); }

            const_iterator& operator++()
            {
                m_Current = m_Parent->m_Objects[m_Current].Next;
                return *this;
            }

            const_iterator operator++(int)
            {
                auto copy = *this;
                m_Current = m_Parent->m_Objects[m_Current].Next;

                return copy;
            }

            friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) = default;
            friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) = default;
        };

        iterator       begin() { return iterator(this, m_FirstOccupied); }
        const_iterator begin() const { return iterator(this, m_FirstOccupied); }
        const_iterator cbegin() const { return iterator(this, m_FirstOccupied); }

        iterator       end() { return iterator(this, m_Objects.size()); }
        const_iterator end() const { return iterator(this, m_Objects.size()); }
        const_iterator cend() const { return iterator(this, m_Objects.size()); }

        PoolPointer<T> Create() { return Create(T()); }

        PoolPointer<T> Create(T&& obj)
        {
            if (m_FirstFree == m_Objects.size())
            {
                expand(m_Objects.size() + DEFAULT_SIZE_INCREASE);
            }

            size_t newObjLoc    = m_FirstFree;
            size_t previousFull = 0;
            size_t nextFull     = 0;

            if (m_FirstOccupied == m_Objects.size())
            {
                previousFull = m_Objects.size();
                nextFull     = m_Objects.size();
            }
            else
            {
                previousFull = findPrevious(m_FirstOccupied, newObjLoc);
                nextFull     = m_Objects[previousFull].Next;
            }

            size_t newNextFree = m_Objects[m_FirstFree].Next;

            m_Objects[newObjLoc].Next = nextFull;
            m_Objects[newObjLoc].Value.emplace(std::forward<T>(obj));

            if (previousFull < m_Objects.size())
            {
                m_Objects[previousFull].Next = newObjLoc;
            }

            m_FirstFree = newNextFree;
            if (newObjLoc < m_FirstOccupied)
            {
                m_FirstOccupied = newObjLoc;
            }

            return PoolPointer<T>(newObjLoc, this);
        }

        template <class... Args>
        PoolPointer<T> Create(Args... args)
        {
            return Create(T(std::forward<Args>(args)...));
        }

        PoolPointer<T> Create(const T& value) { return Create(T(value)); }

      private:
        void destroy(size_t id)
        {
            assert(id < m_Objects.size());
            assert(m_FirstOccupied < m_Objects.size());

            m_Objects[id].Value.reset();

            if (id == m_FirstOccupied)
            {
                size_t nextFull = m_Objects[m_FirstOccupied].Next;
                m_FirstOccupied = nextFull;
            }
            else
            {
                size_t previousFull          = findPrevious(m_FirstOccupied, id);
                m_Objects[previousFull].Next = m_Objects[id].Next;
            }

            if (id < m_FirstFree)
            {
                m_Objects[id].Next = m_FirstFree;
                m_FirstFree        = id;
            }
            else
            {
                size_t previousEmpty = findPrevious(m_FirstFree, id);

                m_Objects[id].Next            = m_Objects[previousEmpty].Next;
                m_Objects[previousEmpty].Next = id;
            }
        }

        void expand(size_t newSize)
        {
            assert(newSize > m_Objects.size());
            size_t oldSize = m_Objects.size();
            m_Objects.resize(newSize);

            size_t previousEmpty = findPrevious(m_FirstFree, oldSize);
            size_t previousFull  = findPrevious(m_FirstOccupied, oldSize);

            m_Objects[previousFull].Next = m_Objects.size();

            m_Objects[previousEmpty].Next = oldSize;
            m_Objects[oldSize].Next       = oldSize + 1;

            for (size_t i = oldSize + 1; i < m_Objects.size(); ++i)
            {
                m_Objects[i].Next = i + 1;
            }
        }

        size_t findPrevious(size_t first, size_t current)
        {
            assert(first < current);
            size_t previous = first;

            while (m_Objects[previous].Next < current)
            {
                previous = m_Objects[previous].Next;
            }
            return previous;
        }
    };
} // namespace Ignosi::Libraries::Containers
