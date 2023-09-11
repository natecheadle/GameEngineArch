#pragma once

#include "HitShape.h"
#include "KinematicData.h"

#include <LinearAlgebra/Vector2.hpp>

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

namespace Ignosi::Modules::Physics
{
    class PhysicsSystem;

    class RigidBody2D
    {
        std::unique_ptr<Physics::HitShape> m_HitShape;
        bool                               m_IsFixed{true};
        KinematicData*                     m_pPosition;

        std::function<void(const RigidBody2D& other)> m_OnCollision;

        friend PhysicsSystem;

      public:
        RigidBody2D() = default;
        RigidBody2D(KinematicData* pPosition);

        void Update(float dt);

        void AttachOnCollision(std::function<void(const RigidBody2D& other)>&& func) { m_OnCollision = std::move(func); }
        void ClearCallbacks();

        void IsFixed(bool val) { m_IsFixed = val; }
        bool IsFixed() const { return m_IsFixed; }

        const Vector3<float>& Position() const { return m_pPosition->Position(); }
        const Vector3<float>& Velocity() const { return m_pPosition->Velocity(); }

        void                                      HitShape(std::unique_ptr<Physics::HitShape> pVal) { m_HitShape = std::move(pVal); }
        const std::unique_ptr<Physics::HitShape>& HitShape() const { return m_HitShape; }

      private:
        void CollisionOccurred(const RigidBody2D& other);
    };
} // namespace Ignosi::Modules::Physics
