#pragma once

#include "IEntity.h"
#include "IWorld.h"
#include "Tag.h"

#include <string_view>
#include <tuple>

namespace Ignosi::Modules::ECS
{
    struct SystemID
    {
        size_t ID;

        static inline SystemID  Create() { return {.ID = RESET_VAL}; }
        inline void             Reset() { ID = RESET_VAL; }
        static constexpr size_t RESET_VAL = std::numeric_limits<size_t>::max();

        friend bool        operator==(const SystemID& lhs, const SystemID& rhs) = default;
        friend inline bool operator!=(const SystemID& lhs, const SystemID& rhs) { return !(lhs == rhs); }
        friend inline bool operator<(const SystemID& lhs, const SystemID& rhs) { return lhs.ID < rhs.ID; }
        friend inline bool operator<=(const SystemID& lhs, const SystemID& rhs) { return lhs < rhs || lhs == rhs; }
        friend inline bool operator>(const SystemID& lhs, const SystemID& rhs) { return lhs.ID > rhs.ID; }
        friend inline bool operator>=(const SystemID& lhs, const SystemID& rhs) { return lhs > rhs || lhs == rhs; }
    };

    class ISystem
    {
      public:
        virtual ~ISystem() = default;

        virtual SystemID ID() const                              = 0;
        virtual void     Initialize(SystemID id, IWorld* pWorld) = 0;

        virtual std::string_view Name() const                                         = 0;
        virtual const Tag&       Tag() const                                          = 0;
        virtual std::uint32_t    Priority() const                                     = 0;
        virtual void             Update(double dt)                                    = 0;
        virtual bool             EntityHasNecessaryComponents(IEntity* pEntity) const = 0;
    };
} // namespace Ignosi::Modules::ECS
