#include "Resource.h"

namespace Ignosi::Modules::ECS
{

    Resource::Resource(const std::filesystem::path& path)
        : m_ResourcePath(path)
    {
    }
} // namespace Ignosi::Modules::ECS
