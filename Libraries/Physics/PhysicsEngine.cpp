#include "PhysicsEngine.h"

#include "LinearAlgebra/Vector.hpp"

using namespace std::chrono_literals;

namespace Ignosi::Libraries::Physics
{
    void PhysicsEngine::Update(std::chrono::milliseconds delta)
    {
        double sec = delta / 1.0s;
        for (auto& value : ComponentPool())
        {
            value.Data().Position(value.Data().Velocity() * sec);
            value.Data().Velocity(value.Data().Acceleration() * sec);
        }
    }
} // namespace Ignosi::Libraries::Physics
