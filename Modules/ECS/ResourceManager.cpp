#include "ResourceManager.h"

#include <cassert>
#include <memory>
#include <string_view>

namespace Ignosi::Modules::ECS
{
    Resource* ResourceManager::LoadResource(std::unique_ptr<Resource> pResource)
    {
        assert(m_Resources.find(pResource->ID().Name) == m_Resources.end());
        Resource* returnVal = pResource.get();
        m_Resources.insert({pResource->ID().Name, std::move(pResource)});

        return returnVal;
    }

    std::unique_ptr<Resource> ResourceManager::UnloadResource(std::string_view name)
    {
        auto it = m_Resources.find(name);
        if (it != m_Resources.end())
        {
            std::unique_ptr<Resource> returnVal = std::move(it->second);
            m_Resources.erase(it);
            return returnVal;
        }
        return nullptr;
    }

    Resource* ResourceManager::GetResource(std::string_view name) const
    {
        auto it = m_Resources.find(name);
        if (it != m_Resources.end())
        {
            return it->second.get();
        }
        return nullptr;
    }
} // namespace Ignosi::Modules::ECS
