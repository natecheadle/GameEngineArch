#pragma once

#include "PhysicsData.h"

#include <System.hpp>

namespace Ignosi::Libraries::Physics
{
    class IPhysicsEngine : public ECS::System<PhysicsData>
    {
    };
} // namespace Ignosi::Libraries::Physics
