#pragma once

#include "ComponentPointer.h"
#include "EntityPointer.h"

using namespace std::placeholders;

namespace Ignosi::Modules::ECS
{
    template <ComponentObject... ComponentTypes>
    class CustomEntity
    {
        EntityPointer<ComponentTypes...> m_pEntity;

      protected:
        CustomEntity() = default;
        CustomEntity(EntityPointer<ComponentTypes...> pEntity)
            : m_pEntity(std::move(pEntity))
        {
            m_pEntity->AttachOnUpdate(std::bind(&CustomEntity<ComponentTypes...>::OnUpdate, this, _1));
        }

        CustomEntity(CustomEntity& other)           = delete;
        CustomEntity(CustomEntity&& other) noexcept = default;

        CustomEntity& operator=(const CustomEntity& other)     = delete;
        CustomEntity& operator=(CustomEntity&& other) noexcept = default;

      public:
        virtual ~CustomEntity()
        {
            if (m_pEntity.IsValid())
            {
                m_pEntity->AttachOnUpdate(nullptr);
            }
        }

        const IEntity*          Entity() const { return m_pEntity.get(); }
        EntityID                ID() const { return m_pEntity->ID(); }
        bool                    IsAlive() const { return m_pEntity->IsAlive(); }
        const std::vector<Tag>& Tags() const { return m_pEntity->Tags(); }

      protected:
        template <ComponentObject Component>
        WeakComponentPointer<Component> GetComponent() const
        {
            return m_pEntity->template GetComponent<Component>();
        }

        virtual void OnUpdate(double dt) = 0;
    };
} // namespace Ignosi::Modules::ECS