#pragma once

#include "Entity.h"
#include "Tag.h"
#include "World.h"

#include <vector>

namespace Ignosi::Modules::ECS
{
    template <typename... ComponentTypes>
    class Scene
    {
        Tag                       m_SceneTag;
        World<ComponentTypes...>* m_pWorld;

      public:
        Scene(const Tag& sceneTag, World<ComponentTypes...>* pWorld)
            : m_SceneTag(sceneTag)
            , m_pWorld(pWorld)
        {
        }

        Scene(std::string_view sceneTagName, World<ComponentTypes...>* pWorld)
            : m_SceneTag(Tag::Create(sceneTagName))
            , m_pWorld(pWorld)
        {
        }

        bool AddEntity(Entity<ComponentTypes...>* pEntity) { return m_pWorld->AddTag(m_SceneTag, pEntity); }
        bool RemoveEntity(Entity<ComponentTypes...>* pEntity) { return m_pWorld->RemoveTag(m_SceneTag, pEntity); }

        const std::vector<Entity<ComponentTypes...>*>& Entities() const
        {
            return m_pWorld->GetEntitiesByTag(m_SceneTag);
        }
    };
} // namespace Ignosi::Modules::ECS
