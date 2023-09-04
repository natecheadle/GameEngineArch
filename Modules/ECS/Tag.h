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
        std::uint32_t                   m_ID;
        std::string_view                m_Name;

        Tag(size_t id, std::string_view name);

      public:
        static Tag Create(std::string_view name);

        Tag() = default;

        Tag(const Tag& other) = default;
        Tag(Tag&& other)      = default;

        Tag& operator=(const Tag& other) = default;
        Tag& operator=(Tag&& other)      = default;

        std::uint32_t    ID() const { return m_ID; }
        std::string_view Name() const { return m_Name; }

        bool operator==(const Tag& other) const { return m_ID == other.m_ID; }
        bool operator!=(const Tag& other) const { return !(*this == other); }
        bool operator<(const Tag& other) const { return m_ID < other.m_ID; }
        bool operator>(const Tag& other) const { return m_ID > other.m_ID; }
        bool operator<=(const Tag& other) const { return m_ID <= other.m_ID; }
        bool operator>=(const Tag& other) const { return m_ID >= other.m_ID; }
    };
} // namespace Ignosi::Modules::ECS
