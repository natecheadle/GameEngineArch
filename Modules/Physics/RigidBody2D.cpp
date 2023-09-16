#include "RigidBody2D.h"

#include "LinearAlgebra/Vector3.hpp"

#include <memory>

namespace Ignosi::Modules::Physics
{
    RigidBody2D::RigidBody2D(ECS::WeakComponentPointer<Physics::KinematicData> pPosition)
        : m_pPosition(pPosition)
        , m_pOnCollisionEvent(std::make_unique<Messaging::Event<const RigidBody2D&>>())
    {
    }

    void RigidBody2D::Update(float dt) {}

    void RigidBody2D::CollisionOccurred(const RigidBody2D& other)
    {
        assert(m_pOnCollisionEvent);
        m_pOnCollisionEvent->Invoke(other);
    }

} // namespace Ignosi::Modules::Physics
