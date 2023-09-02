#pragma once

#include "Tag.h"

#include <cstdint>
#include <functional>
#include <vector>

namespace Ignosi::Modules::ECS
{
    struct EntityID
    {
        size_t ID;

        static inline EntityID  Create() { return {.ID = RESET_VAL}; }
        inline void             Reset() { ID = RESET_VAL; }
        static constexpr size_t RESET_VAL = std::numeric_limits<size_t>::max();

        friend bool        operator==(const EntityID& lhs, const EntityID& rhs) = default;
        friend inline bool operator!=(const EntityID& lhs, const EntityID& rhs) { return !(lhs == rhs); }
        friend inline bool operator<(const EntityID& lhs, const EntityID& rhs) { return lhs.ID < rhs.ID; }
        friend inline bool operator<=(const EntityID& lhs, const EntityID& rhs) { return lhs < rhs || lhs == rhs; }
        friend inline bool operator>(const EntityID& lhs, const EntityID& rhs) { return lhs.ID > rhs.ID; }
        friend inline bool operator>=(const EntityID& lhs, const EntityID& rhs) { return lhs > rhs || lhs == rhs; }
    };

    class IEntity
    {
      public:
        virtual EntityID                ID() const                                           = 0;
        virtual bool                    IsAlive() const                                      = 0;
        virtual void                    Update(double dt)                                    = 0;
        virtual void                    AttachOnUpdate(std::function<void(double)> callback) = 0;
        virtual const std::vector<Tag>& Tags() const                                         = 0;
    };
} // namespace Ignosi::Modules::ECS
