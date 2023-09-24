#pragma once

#include <UID.hpp>

#include <cstdint>
#include <filesystem>

namespace Ignosi::Modules::ECS
{
    struct ResourceID
    {
        std::uint64_t ID = UID();
        std::string   Name;
    };

    class Resource
    {
        ResourceID m_ID;

      protected:
        Resource(const std::string& name);

      public:
        virtual ~Resource() = default;

        Resource(const Resource& other)     = delete;
        Resource(Resource&& other) noexcept = default;

        Resource& operator=(const Resource& other)     = delete;
        Resource& operator=(Resource&& other) noexcept = default;

        const ResourceID& ID() const { return m_ID; }
    };
} // namespace Ignosi::Modules::ECS
