#pragma once

#include "EntityPointer.h"

using namespace std::placeholders;

namespace Ignosi::Modules::ECS
{
    template <typename... ComponentTypes>
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
        virtual ~CustomEntity() { m_pEntity->AttachOnUpdate(nullptr); }

        EntityID                ID() const { return m_pEntity->ID(); }
        bool                    IsAlive() const { return m_pEntity->IsAlive(); }
        const std::vector<Tag>& Tags() const { return m_pEntity->Tags(); }

      protected:
        template <typename Component>
        Component* Get()
        {
            return m_pEntity->template Get<Component>();
        }

        template <typename Component>
        const Component* Get() const
        {
            return m_pEntity->template Get<Component>();
        }
        virtual void OnUpdate(double dt) = 0;
    };
} // namespace Ignosi::Modules::ECS