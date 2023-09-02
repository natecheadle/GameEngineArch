#pragma once

#include <cstddef>
#include <limits>

namespace Ignosi::Modules::ECS
{
    struct ComponentID
    {
        size_t ID;

        static inline ComponentID Create() { return {.ID = RESET_VAL}; }
        inline void               Reset() { ID = RESET_VAL; }
        static constexpr size_t   RESET_VAL = std::numeric_limits<size_t>::max();

        friend bool        operator==(const ComponentID& lhs, const ComponentID& rhs) = default;
        friend inline bool operator!=(const ComponentID& lhs, const ComponentID& rhs) { return !(lhs == rhs); }
        friend inline bool operator<(const ComponentID& lhs, const ComponentID& rhs) { return lhs.ID < rhs.ID; }
        friend inline bool operator<=(const ComponentID& lhs, const ComponentID& rhs) { return lhs < rhs || lhs == rhs; }
        friend inline bool operator>(const ComponentID& lhs, const ComponentID& rhs) { return lhs.ID > rhs.ID; }
        friend inline bool operator>=(const ComponentID& lhs, const ComponentID& rhs) { return lhs > rhs || lhs == rhs; }
    };
} // namespace Ignosi::Modules::ECS