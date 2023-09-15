#include "RigidBody2D.h"

#include "LinearAlgebra/Vector3.hpp"

namespace Ignosi::Modules::Physics
{
    RigidBody2D::RigidBody2D(ECS::WeakComponentPointer<Physics::KinematicData> pPosition)
        : m_pPosition(pPosition)
    {
    }

    void RigidBody2D::Update(float dt) {}

    void RigidBody2D::CollisionOccurred(const RigidBody2D& other)
    {
        m_OnCollisionEvent(other);
    }

} // namespace Ignosi::Modules::Physics
