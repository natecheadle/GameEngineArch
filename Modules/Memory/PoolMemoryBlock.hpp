#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <vector>

namespace nate::Modules::Memory
{
    template <class T>
    class PoolMemoryBlock
    {
        struct Data
        {
            bool                                IsEmpty{true};
            size_t                              PreviousObject{0};
            size_t                              NextObject{0};
            std::array<std::uint8_t, sizeof(T)> Data;
        };

        std::map<void*, std::function<void()>>        m_OnDestroySubs;
        std::map<size_t, std::function<void(size_t)>> m_OnMoveSubs;

        std::vector<Data> m_Data;
        size_t            m_FirstDataIndex;
        size_t            m_FirstEmptyIndex;
        size_t            m_UsedSize;

      public:
        class pointer
        {
            friend PoolMemoryBlock<T>;

            PoolMemoryBlock<T>* m_pPool;
            size_t              m_PoolIndex;

          public:
            ~pointer()
            {
                if (m_pPool)
                {
                    m_pPool->DestroyObject(m_PoolIndex);
                    m_pPool->UnsubscribeOnDestroy(this);
                    m_pPool->UnsubscribeOnMove(m_PoolIndex);
                }
            }

            pointer(const pointer& other)            = delete;
            pointer& operator=(const pointer& other) = delete;

            pointer(pointer&& other)
                : m_PoolIndex(other.m_PoolIndex)
                , m_pPool(other.m_pPool)
            {
                other.m_pPool = nullptr;
                if (m_pPool)
                {
                    m_pPool->UnsubscribeOnDestroy(&other);
                    m_pPool->SubscribeOnDestroy(this, [this]() { OnPoolParentDestroyed(); });

                    m_pPool->UnsubscribeOnMove(m_PoolIndex);
                    m_pPool->SubscribeOnMove(m_PoolIndex, [this](size_t newIndex) { OnObjectMoved(newIndex); });
                }
            }

            pointer& operator=(pointer&& other)
            {
                if (this == &other)
                    return *this;

                m_PoolIndex = other.m_PoolIndex;
                m_pPool     = other.m_pPool;

                other.m_pPool = nullptr;
                m_pPool->UnsubscribeOnDestroy(&other);
                m_pPool->SubscribeOnDestroy(this, this->PoolParentDestroyed());

                m_pPool->UnsubscribeOnMove(m_PoolIndex);
                m_pPool->SubscribeOnMove(m_PoolIndex, [this](size_t newIndex) { OnObjectMoved(newIndex); });

                return *this;
            }

            bool IsPoolDestroyed() { return !m_pPool; }

            T*       operator->() { return get(); }
            const T* operator->() const { return get(); }

            T& operator*()
            {
                assert(m_pPool);
                assert(!m_pPool->m_Data[m_PoolIndex].IsEmpty);
                return *(m_pPool->ObjectAt(m_PoolIndex));
            }

            const T& operator*() const
            {
                assert(m_pPool);
                assert(!m_pPool->m_Data[m_PoolIndex].IsEmpty);
                return *(m_pPool->ObjectAt(m_PoolIndex));
            }

            T* get()
            {
                if (!m_pPool)
                    return nullptr;

                assert(!m_pPool->m_Data[m_PoolIndex].IsEmpty);
                return m_pPool->ObjectAt(m_PoolIndex);
            }
            const T* get() const
            {
                if (!m_pPool)
                    return nullptr;

                assert(!m_pPool->m_Data[m_PoolIndex].IsEmpty);
                return m_pPool->ObjectAt(m_PoolIndex);
            }

          private:
            pointer(PoolMemoryBlock<T>* pPool, size_t poolIndex)
                : m_pPool(pPool)
                , m_PoolIndex(poolIndex)
            {
                if (m_pPool)
                {
                    m_pPool->SubscribeOnDestroy(this, [this]() { OnPoolParentDestroyed(); });
                    m_pPool->SubscribeOnMove(m_PoolIndex, [this](size_t newIndex) { OnObjectMoved(newIndex); });
                }
            }

            void OnPoolParentDestroyed() { m_pPool = nullptr; }
            void OnObjectMoved(size_t newLoc) { m_PoolIndex = newLoc; }
        };

        class iterator
        {
          public:
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = T;
            using pointer           = T*;
            using const_pointer     = const T*;
            using reference         = T&;
            using const_reference   = const T&;

          private:
            PoolMemoryBlock<T>* m_pPool;
            size_t              m_Index;

          public:
            iterator(PoolMemoryBlock<T>* pPool, size_t i)
                : m_pPool(pPool)
                , m_Index(i)
            {
            }

            const_reference operator*() const { return *(m_pPool->ObjectAt(m_Index)); }
            reference       operator*() { return *(m_pPool->ObjectAt(m_Index)); }

            const_pointer operator->() const { return m_pPool->ObjectAt(m_Index); }
            pointer       operator->() { return m_pPool->ObjectAt(m_Index); }

            iterator& operator++()
            {
                m_Index = m_pPool->m_Data[m_Index].NextObject;
                return *this;
            }

            iterator operator++(int)
            {
                iterator tmp = *this;
                m_Index      = m_pPool->m_Data[m_Index].NextObject;
                return tmp;
            }

            iterator& operator--()
            {
                m_Index = m_pPool->m_Data[m_Index].PreviousObject;
                return *this;
            }

            iterator operator--(int)
            {
                iterator tmp = *this;
                m_Index      = m_pPool->m_Data[m_Index].PreviousObject;
                return tmp;
            }

            friend bool operator==(const iterator& a, const iterator& b)
            {
                return a.m_Index == b.m_Index && a.m_pPool == b.m_pPool;
            };
            friend bool operator!=(const iterator& a, const iterator& b) { return !(a == b); };
        };

        class const_iterator
        {
          public:
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = T;
            using const_pointer     = const T*;
            using const_reference   = const T&;

          private:
            PoolMemoryBlock<T>* m_pPool;
            size_t              m_Index;

          public:
            const_iterator(PoolMemoryBlock<T>* pPool, size_t i)
                : m_pPool(pPool)
                , m_Index(i)
            {
            }

            const_reference operator*() const { return *(m_pPool->ObjectAt(m_Index)); }
            const_pointer   operator->() const { return m_pPool->ObjectAt(m_Index); }

            const_iterator& operator++()
            {
                m_Index = m_pPool->m_Data[m_Index].NextObject;
                return *this;
            }

            const_iterator operator++(int)
            {
                iterator tmp = *this;
                m_Index      = m_pPool->m_Data[m_Index].NextObject;
                return tmp;
            }

            const_iterator& operator--()
            {
                m_Index = m_pPool->m_Data[m_Index].PreviousObject;
                return *this;
            }

            const_iterator operator--(int)
            {
                iterator tmp = *this;
                m_Index      = m_pPool->m_Data[m_Index].PreviousObject;
                return tmp;
            }

            friend bool operator==(const const_iterator& a, const const_iterator& b)
            {
                return a.m_Index == b.m_Index && a.m_pPool == b.m_pPool;
            };
            friend bool operator!=(const const_iterator& a, const const_iterator& b) { return !(a == b); };
        };

        PoolMemoryBlock(size_t initSize = 64)
            : m_Data(initSize)
            , m_FirstDataIndex(initSize)
            , m_FirstEmptyIndex(0)
            , m_UsedSize(0)
        {
            m_Data.front().PreviousObject = m_Data.size();
            for (size_t i = 1; i < m_Data.size(); ++i)
            {
                m_Data[i - 1].NextObject = i;
                m_Data[i].PreviousObject = i - 1;
            }
            m_Data.back().NextObject = m_Data.size();
        }

        ~PoolMemoryBlock()
        {
            for (auto& sub : m_OnDestroySubs)
            {
                sub.second();
            }
        }

        template <typename... Args>
        pointer CreateObject(Args&&... args)
        {
            // Pool is full
            if (m_FirstEmptyIndex == m_Data.size())
            {
                size_t currentSize = m_Data.size();
                m_Data.resize(currentSize * 2);
                for (size_t i = currentSize; i < m_Data.size(); ++i)
                {
                    m_Data[i].IsEmpty        = true;
                    m_Data[i - 1].NextObject = i;
                    m_Data[i].PreviousObject = i - 1;
                }
                m_Data[currentSize].PreviousObject = m_Data.size();
                m_Data[currentSize - 1].NextObject = m_Data.size();
                m_Data.back().NextObject           = m_Data.size();
            }

            size_t  newObjLocation = m_FirstEmptyIndex;
            pointer newObj(this, newObjLocation);
            m_Data[newObjLocation].IsEmpty = false;
            m_UsedSize++;

            // Call Constructor at location
            // Pool Pointer is holding the object, but is using the
            // array index instead of a pointer.
            new (reinterpret_cast<void*>(m_Data[newObjLocation].Data.data())) T(std::forward<Args>(std::move(args))...);

            // Pool only has one empty space.
            if (m_Data[m_FirstEmptyIndex].NextObject == m_Data.size())
            {
                m_FirstEmptyIndex = m_Data.size();
            }
            else
            {
                m_FirstEmptyIndex = m_Data[newObjLocation].NextObject;
            }

            // Pool is empty
            if (m_FirstDataIndex == m_Data.size())
            {
                m_Data[newObjLocation].PreviousObject = m_Data.size();
                m_Data[newObjLocation].NextObject     = m_Data.size();
                m_FirstDataIndex                      = newObjLocation;
            }
            else if (m_FirstDataIndex > newObjLocation)
            {
                m_Data[newObjLocation].PreviousObject   = m_Data.size();
                m_Data[newObjLocation].NextObject       = newObjLocation;
                m_Data[m_FirstDataIndex].PreviousObject = newObjLocation;
                m_FirstDataIndex                        = newObjLocation;
            }
            else
            {
                size_t previousObj                    = FindPreviousRealObject(newObjLocation);
                m_Data[newObjLocation].PreviousObject = previousObj;
                m_Data[newObjLocation].NextObject     = m_Data[previousObj].NextObject;
                m_Data[previousObj].NextObject        = newObjLocation;
            }

            return newObj;
        }

        size_t UsedSize() const { return m_UsedSize; }
        size_t MaxSize() const { return m_Data.size(); }
        bool   IsFragmented() const { return m_FirstEmptyIndex != m_UsedSize; }
        void   Defragment()
        {
            // Already Defragrmented
            if (!IsFragmented())
                return;

            std::map<size_t, std::function<void(size_t)>> oldSubs(std::move(m_OnMoveSubs));
            m_OnMoveSubs = std::map<size_t, std::function<void(size_t)>>();
            size_t newIndex{0};
            for (size_t i = 0; i < m_Data.size(); ++i)
            {
                if (!m_Data[i].IsEmpty)
                {
                    m_Data[newIndex].IsEmpty        = false;
                    m_Data[newIndex].PreviousObject = newIndex - 1;
                    m_Data[newIndex].NextObject     = newIndex + 1;

                    auto sub = oldSubs.find(i);
                    if (sub != oldSubs.end())
                    {
                        sub->second(newIndex);
                        m_OnMoveSubs.insert({newIndex, std::move(sub->second)});
                    }
                    newIndex++;
                }
            }

            m_Data.front().PreviousObject     = m_Data.size();
            m_Data[m_UsedSize - 1].NextObject = m_Data.size();

            for (size_t i = m_UsedSize; i < m_Data.size(); ++i)
            {
                m_Data[i].IsEmpty        = true;
                m_Data[i].PreviousObject = i - 1;
                m_Data[i].NextObject     = i + 1;
            }
            m_Data[m_UsedSize].PreviousObject = m_Data.size();
            m_Data.back().NextObject          = m_Data.size();

            m_FirstEmptyIndex = m_UsedSize;
            m_FirstDataIndex  = 0;
        }

        iterator       begin() { return iterator(this, m_FirstDataIndex); }
        iterator       end() { return iterator(this, m_Data.size()); }
        const_iterator cbegin() { return const_iterator(this, m_FirstDataIndex); }
        const_iterator cend() { return const_iterator(this, m_Data.size()); }

      private:
        T*       ObjectAt(size_t i) { return reinterpret_cast<T*>(m_Data[i].Data.data()); }
        const T* ObjectAt(size_t i) const { return reinterpret_cast<const T*>(m_Data[i].Data.data()); }

        void DestroyObject(size_t i)
        {
            if (m_Data[i].IsEmpty)
            {
                return;
            }
            m_UsedSize--;

            ObjectAt(i)->~T();

            if (i == m_FirstDataIndex)
            {
                m_FirstDataIndex = m_Data[i].NextObject;
            }

            if (m_Data[i].PreviousObject < m_Data.size())
            {
                m_Data[m_Data[i].PreviousObject].NextObject = m_Data[i].NextObject;
            }
            if (m_Data[i].NextObject < m_Data.size())
            {
                m_Data[m_Data[i].NextObject].PreviousObject = m_Data[i].PreviousObject;
            }

            if (i < m_FirstEmptyIndex)
            {
                m_Data[i].PreviousObject = m_Data.size();
                m_Data[i].NextObject     = m_FirstEmptyIndex;
                m_FirstEmptyIndex        = i;
            }
            else
            {
                size_t prevObj           = FindPreviousEmptyObject(i);
                m_Data[i].PreviousObject = prevObj;
                if (prevObj != m_Data.size())
                {
                    m_Data[i].NextObject       = m_Data[prevObj].NextObject;
                    m_Data[prevObj].NextObject = i;
                }
                else
                {
                    m_Data[i].NextObject = m_Data.size();
                }
            }

            m_Data[i].IsEmpty = true;
        }

        size_t FindPreviousEmptyObject(size_t objI)
        {
            assert(!m_Data[objI].IsEmpty);
            size_t searchI = m_FirstEmptyIndex;
            // This would imply the pool is full.
            if (searchI >= m_Data.size())
                return m_Data.size();

            while (m_Data[searchI].NextObject < objI)
            {
                searchI = m_Data[searchI].NextObject;
            }

            return searchI;
        }

        size_t FindPreviousRealObject(size_t objI)
        {
            assert(!m_Data[objI].IsEmpty);
            size_t searchI = m_FirstDataIndex;
            while (m_Data[searchI].NextObject < objI)
            {
                searchI = m_Data[searchI].NextObject;
            }

            return searchI;
        }

        void SubscribeOnDestroy(void* pSub, std::function<void()> callback)
        {
            m_OnDestroySubs.insert_or_assign(pSub, std::move(callback));
        }

        void UnsubscribeOnDestroy(void* pSub)
        {
            auto it = m_OnDestroySubs.find(pSub);
            if (it != m_OnDestroySubs.end())
            {
                m_OnDestroySubs.erase(it);
            }
        }

        void SubscribeOnMove(size_t sub, std::function<void(size_t)> callback)
        {
            m_OnMoveSubs.insert_or_assign(sub, std::move(callback));
        }

        void UnsubscribeOnMove(size_t sub)
        {
            auto it = m_OnMoveSubs.find(sub);
            if (it != m_OnMoveSubs.end())
            {
                m_OnMoveSubs.erase(it);
            }
        }
    };

    template <class T>
    using pool_pointer = typename PoolMemoryBlock<T>::pointer;
} // namespace nate::Modules::Memory
