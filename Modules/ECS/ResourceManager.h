#pragma once

#include "Resource.h"

#include <filesystem>
#include <map>
#include <memory>

namespace Ignosi::Modules::ECS
{
    class ResourceManager
    {
        std::map<std::filesystem::path, std::unique_ptr<Resource>> m_Resources;

      public:
        ResourceManager() = default;

        ResourceManager(const ResourceManager& other)     = delete;
        ResourceManager(ResourceManager&& other) noexcept = default;

        ResourceManager& operator=(const ResourceManager& other) = delete;
        ResourceManager& operator=(ResourceManager&& other)      = default;

        Resource*                 LoadResource(std::unique_ptr<Resource> pResource);
        std::unique_ptr<Resource> UnloadResource(std::filesystem::path& path);
        Resource*                 GetResource(const std::filesystem::path& path) const;
    };
} // namespace Ignosi::Modules::ECS
