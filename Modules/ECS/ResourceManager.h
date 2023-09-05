#pragma once

#include "Resource.h"

#include <filesystem>
#include <memory>
#include <vector>

namespace Ignosi::Modules::ECS
{
    class ResourceManager
    {
        std::vector<std::unique_ptr<Resource>> m_Resources;

      public:
        ResourceManager() = default;

        template <class T>
        T* RegisterResource(std::unique_ptr<T> pResource)
        {
            assert(nullptr == GetResource(pResource->ResourcePath()));
            T* returnVal = pResource.get();
            m_Resources.push_back(std::move(pResource));

            return returnVal;
        }

        Resource* GetResource(const std::filesystem::path& path) const;
    };
} // namespace Ignosi::Modules::ECS
