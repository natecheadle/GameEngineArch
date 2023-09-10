#include "RigidBody2D.h"

#include "LinearAlgebra/Vector3.hpp"

namespace Ignosi::Modules::Physics
{
    void RigidBody2D::Update(float dt) {}

    void RigidBody2D::CollisionOccurred(const RigidBody2D& other)
    {
        if (m_OnCollision)
            m_OnCollision(other);
    }

    void RigidBody2D::ClearCallbacks()
    {
        m_OnCollision = nullptr;
    }
} // namespace Ignosi::Modules::Physics
