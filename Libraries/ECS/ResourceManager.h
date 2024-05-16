#pragma once

#include "Resource.hpp"

#include <memory>
#include <unordered_map>

namespace Ignosi::Libraries::ECS
{
    class ResourceManager
    {
        std::unordered_map<std::string, std::unique_ptr<IResource>> m_Resources;

      public:
        ResourceManager()          = default;
        virtual ~ResourceManager() = default;

        template <typename T>
        Resource<T>* CreateResource(const std::string& id, std::unique_ptr<T> resource)
        {
            auto it = m_Resources.find(id);
            if (it != m_Resources.end())
            {
                return dynamic_cast<Resource<T>*>(it->second.get());
            }

            auto         pResource = std::make_unique<Resource<T>>(id, std::move(resource));
            Resource<T>* result    = pResource.get();
            m_Resources.insert({id, std::move(pResource)});
            return result;
        }

        template <typename T>
        Resource<T>* GetResource(const std::string& id) const
        {
            auto it = m_Resources.find(id);
            if (it != m_Resources.end())
            {
                return dynamic_cast<Resource<T>*>(it->second.get());
            }
            return nullptr;
        }

        bool DestroyResource(const std::string& id)
        {
            auto it = m_Resources.find(id);
            if (it != m_Resources.end())
            {
                m_Resources.erase(it);
                return true;
            }
            return false;
        }
    };
} // namespace Ignosi::Libraries::ECS
