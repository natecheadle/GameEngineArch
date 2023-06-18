#include "RigidBody2D.h"

namespace nate::Modules::Physics
{
    void RigidBody2D::Update(float dt) {}

    void RigidBody2D::CollisionOccurred(const RigidBody2D& other)
    {
        if (m_OnCollision)
            m_OnCollision(other);
    }

    void RigidBody2D::ClearCallbacks()
    {
        m_OnPosChange = nullptr;
        m_OnCollision = nullptr;
    }
} // namespace nate::Modules::Physics
