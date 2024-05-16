#pragma once

#include "IResource.h"

#include <memory>

namespace Ignosi::Libraries::ECS
{
    template <typename T>
    class Resource : public IResource
    {
        std::string        m_ID;
        std::unique_ptr<T> m_pResource;

      public:
        Resource(std::string id, std::unique_ptr<T> resource)
            : m_ID(std::move(id))
            , m_pResource(std::move(resource))
        {
        }
        virtual ~Resource() = default;

        const std::string& ID() const override { return m_ID; }

        T*       operator->() { return m_pResource.get(); }
        const T* operator->() const { return m_pResource.get(); }

        T&       operator*() { return *(m_pResource.get()); }
        const T& operator*() const { return *(m_pResource.get()); }

        T*       get() { return m_pResource.get(); }
        const T* get() const { return m_pResource.get(); }
    };
} // namespace Ignosi::Libraries::ECS
