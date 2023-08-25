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
        size_t                          m_ID{std::numeric_limits<size_t>::max()};
        std::string_view                m_Name;

        Tag(size_t id, std::string_view name)
            : m_ID(id)
            , m_Name(name)
        {
        }

      public:
        static Tag Create(std::string_view name)
        {
            auto it = std::find(m_TagNames.begin(), m_TagNames.end(), name);
            if (it != m_TagNames.end())
            {
                return Tag(it - m_TagNames.begin(), std::string_view(it->begin(), it->end()));
            }

            m_TagNames.push_back(std::string(name));
            return Tag(m_TagNames.size() - 1, std::string_view(m_TagNames.back().begin(), m_TagNames.back().end()));
        }

        Tag() = default;

        Tag(const Tag& other) = default;
        Tag(Tag&& other)      = default;

        Tag& operator=(const Tag& other) = default;
        Tag& operator=(Tag&& other)      = default;

        size_t           ID() const { return m_ID; }
        std::string_view Name() const { return m_Name; }
        bool             IsValid() const { return m_ID != std::numeric_limits<size_t>::max(); }

        bool operator==(const Tag& other) const { return m_ID == other.m_ID; }
    };
} // namespace Ignosi::Modules::ECS
