#pragma once

#include "ECSObject.hpp"

#include <ObjectPool.hpp>

#include <vector>

namespace Ignosi::Libraries::ECS
{

    template <typename T>
    class ECSPool
    {
        friend ECSObject<T>;

        Containers::ObjectPool<T>               m_Pool;
        std::vector<Containers::PoolPointer<T>> m_ToFree;

      public:
        ECSPool() = default;

        ~ECSPool() = default;

        ECSPool(const ECSPool& other) = delete;
        ECSPool(ECSPool&& other)      = default;

        ECSPool& operator=(const ECSPool& other) = delete;
        ECSPool& operator=(ECSPool&& other)      = default;

        ECSObject<T> Create(T&& obj) { return ECSObject(this, m_Pool.Create(std::forward<T>(obj))); }
        ECSObject<T> Create(const T& obj) { return ECSObject(this, m_Pool.Create(obj)); }

        std::optional<T>&       operator[](size_t id) { return m_Pool[id]; }
        const std::optional<T>& operator[](size_t id) const { return m_Pool[id]; }

        void Flush() { m_ToFree.clear(); }

      private:
        void queueFree(Containers::PoolPointer<T>&& obj) { m_ToFree.push_back(std::forward<Containers::PoolPointer<T>>(obj)); }
    };
} // namespace Ignosi::Libraries::ECS
