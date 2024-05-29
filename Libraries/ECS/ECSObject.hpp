#pragma once

#include <PoolPointer.hpp>
#include <fmt/format.h>

#include <cassert>
#include <utility>

namespace Ignosi::Libraries::ECS
{

    template <typename T>
    class ECSPool;

    template <typename T>
    class ECSObject
    {
        friend ECSPool<T>;

        ECSPool<T>*                m_Parent{nullptr};
        Containers::PoolPointer<T> m_ObjectPointer;

        ECSObject(ECSPool<T>* pParent, Containers::PoolPointer<T>&& obj)
            : m_Parent(pParent)
            , m_ObjectPointer(std::forward<Containers::PoolPointer<T>>(obj))
        {
            fmt::print("Creating object with ID = {} and Parent = {}\n", m_ObjectPointer.ID(), (std::uintptr_t)m_Parent);
        }

      public:
        ECSObject() = default;
        ~ECSObject()
        {
            if (m_Parent && m_ObjectPointer.IsValid())
            {
                fmt::print("Destroying object with ID = {} and Parent = {}\n", m_ObjectPointer.ID(), (std::uintptr_t)m_Parent);
                m_Parent->queueFree(std::move(m_ObjectPointer));
            }
        }

        ECSObject(const ECSObject& other) = delete;
        ECSObject(ECSObject&& other) noexcept
            : m_Parent(other.m_Parent)
            , m_ObjectPointer(std::move(other.m_ObjectPointer))
        {
            fmt::print("Move constructing object with ID = {} and Parent = {}\n", m_ObjectPointer.ID(), (std::uintptr_t)m_Parent);
            other.m_Parent = nullptr;
        }

        ECSObject& operator=(const ECSObject& other) = delete;
        ECSObject& operator=(ECSObject&& other) noexcept
        {
            m_Parent        = other.m_Parent;
            m_ObjectPointer = std::move(other.m_ObjectPointer);
            other.m_Parent  = nullptr;

            fmt::print("Moving object with ID = {} and Parent = {}\n", m_ObjectPointer.ID(), (std::uintptr_t)m_Parent);
            return *this;
        }

        size_t ID() const { return m_ObjectPointer.ID(); }

        T*       operator->() { return m_ObjectPointer.Get(); }
        const T* operator->() const { return m_ObjectPointer.Get(); }

        T&       operator*() { return *(m_ObjectPointer.Get()); }
        const T& operator*() const { return *(m_ObjectPointer.Get()); }

        T&       Data() { return *(m_ObjectPointer.Get()); }
        const T& Data() const { return *(m_ObjectPointer.Get()); }

        T*       Get() { return m_ObjectPointer.Get(); }
        const T* Get() const { return m_ObjectPointer.Get(); }

        bool IsAlive() const { return m_ObjectPointer.IsValid(); }
    };
} // namespace Ignosi::Libraries::ECS
