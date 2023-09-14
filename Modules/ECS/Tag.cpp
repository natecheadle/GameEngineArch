#include "Tag.h"

namespace Ignosi::Modules::ECS
{
    std::vector<std::string> Tag::m_TagNames;

    Tag::Tag::Tag(size_t id, std::string_view name)
        : m_ID(id)
        , m_Name(name)
    {
    }

    Tag Tag::Create(std::string_view name)
    {
        auto it = std::find(m_TagNames.begin(), m_TagNames.end(), name);
        if (it != m_TagNames.end())
        {
            return Tag(static_cast<std::uint32_t>(it - m_TagNames.begin()), std::string_view(it->begin(), it->end()));
        }

        m_TagNames.push_back(std::string(name));
        return Tag(
            static_cast<std::uint32_t>(m_TagNames.size() - 1),
            std::string_view(m_TagNames.back().begin(), m_TagNames.back().end()));
    }
} // namespace Ignosi::Modules::ECS
