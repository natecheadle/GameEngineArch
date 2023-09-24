#pragma once

#include "ClassID.hpp"
#include "IEntity.h"
#include "Tag.h"

#include <string_view>
#include <tuple>

namespace Ignosi::Modules::ECS
{
    class IWorld;

    class ComponentID : public ClassID<ComponentID>
    {
      public:
        ComponentID() = default;
        ComponentID(size_t id)
            : ClassID<ComponentID>(id)
        {
        }
    };

    class IComponent
    {
      public:
        virtual ~IComponent() = default;

        virtual const Tag& Tag() const = 0;
    };
    template <typename T>
    concept ComponentObject = std::is_base_of_v<IComponent, T>;

} // namespace Ignosi::Modules::ECS
