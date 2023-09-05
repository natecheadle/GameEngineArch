#include "ResourceManager.h"

namespace Ignosi::Modules::ECS
{
    Resource* ResourceManager::GetResource(const std::filesystem::path& path) const
    {
        for (const auto& resource : m_Resources)
        {
            if (resource->ResourcePath() == path)
            {
                return resource.get();
            }
        }

        return nullptr;
    }
} // namespace Ignosi::Modules::ECS
