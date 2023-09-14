#include "ResourceManager.h"

#include <memory>

namespace Ignosi::Modules::ECS
{
    Resource* ResourceManager::LoadResource(std::unique_ptr<Resource> pResource)
    {
        assert(m_Resources.find(pResource->ResourcePath()) == m_Resources.end());
        Resource* returnVal = pResource.get();
        m_Resources.insert({pResource->ResourcePath(), std::move(pResource)});

        return returnVal;
    }

    std::unique_ptr<Resource> ResourceManager::UnloadResource(std::filesystem::path& path)
    {
        auto it = m_Resources.find(path);
        if (it != m_Resources.end())
        {
            std::unique_ptr<Resource> returnVal = std::move(it->second);
            m_Resources.erase(it);
            return returnVal;
        }
        return nullptr;
    }

    Resource* ResourceManager::GetResource(const std::filesystem::path& path) const
    {
        auto it = m_Resources.find(path);
        if (it != m_Resources.end())
        {
            return it->second.get();
        }
        return nullptr;
    }
} // namespace Ignosi::Modules::ECS
