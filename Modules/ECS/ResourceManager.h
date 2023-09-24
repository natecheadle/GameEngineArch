#pragma once

#include "Resource.h"

#include <filesystem>
#include <map>
#include <memory>
#include <string_view>

namespace Ignosi::Modules::ECS
{
    class ResourceManager
    {
        std::map<std::string_view, std::unique_ptr<Resource>> m_Resources;

      public:
        ResourceManager() = default;

        ResourceManager(const ResourceManager& other)     = delete;
        ResourceManager(ResourceManager&& other) noexcept = default;

        ResourceManager& operator=(const ResourceManager& other) = delete;
        ResourceManager& operator=(ResourceManager&& other)      = default;

        Resource*                 LoadResource(std::unique_ptr<Resource> pResource);
        std::unique_ptr<Resource> UnloadResource(std::string_view name);
        Resource*                 GetResource(std::string_view name) const;
    };
} // namespace Ignosi::Modules::ECS
