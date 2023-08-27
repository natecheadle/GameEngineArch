#pragma once

#include <cstddef>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

namespace Ignosi::Modules::ECS
{
    class Tag
    {
        static std::vector<std::string> m_TagNames;
        size_t                          m_ID;
        std::string_view                m_Name;

        Tag(size_t id, std::string_view name);

      public:
        static Tag Create(std::string_view name);

        Tag() = default;

        Tag(const Tag& other) = default;
        Tag(Tag&& other)      = default;

        Tag& operator=(const Tag& other) = default;
        Tag& operator=(Tag&& other)      = default;

        size_t           ID() const { return m_ID; }
        std::string_view Name() const { return m_Name; }

        bool operator==(const Tag& other) const { return m_ID == other.m_ID; }
    };
} // namespace Ignosi::Modules::ECS
