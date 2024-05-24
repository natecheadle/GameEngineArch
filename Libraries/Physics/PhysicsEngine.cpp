#include "PhysicsEngine.h"

#include "LinearAlgebra/Vector3.hpp"

using namespace std::chrono_literals;

namespace Ignosi::Libraries::Physics
{
    void PhysicsEngine::Update(std::chrono::milliseconds delta)
    {
        double sec = delta / 1.0s;
        for (auto& value : ComponentPool())
        {
            Math::Vector3<double> newPosition(value.Data().Velocity() * sec);
            Math::Vector3<double> newVelocity(value.Data().Acceleration() * sec);
            value.Data().Position(newPosition);
            value.Data().Velocity(newVelocity);
        }
    }
} // namespace Ignosi::Libraries::Physics
