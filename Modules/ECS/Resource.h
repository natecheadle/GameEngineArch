#pragma once

#include <filesystem>

namespace Ignosi::Modules::ECS
{
    class Resource
    {
        std::filesystem::path m_ResourcePath;

      protected:
        Resource(std::filesystem::path path);

      public:
        virtual ~Resource() = default;

        Resource(const Resource& other)     = delete;
        Resource(Resource&& other) noexcept = delete;

        Resource& operator=(const Resource& other)     = delete;
        Resource& operator=(Resource&& other) noexcept = delete;

        const std::filesystem::path& ResourcePath() const { return m_ResourcePath; }
    };
} // namespace Ignosi::Modules::ECS
