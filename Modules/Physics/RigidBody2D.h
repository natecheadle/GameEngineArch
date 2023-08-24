#pragma once

#include "HitShape.h"

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
        Vector2<float>                     m_Position;
        Vector2<float>                     m_Velocity;
        std::unique_ptr<Physics::HitShape> m_HitShape;
        bool                               m_IsFixed{true};

        std::function<void(const Vector2<float>&)>    m_OnPosChange;
        std::function<void(const RigidBody2D& other)> m_OnCollision;

        friend PhysicsSystem;

      public:
        void Update(float dt);

        void AttachOnPosChange(std::function<void(const Vector2<float>&)>&& func) { m_OnPosChange = std::move(func); }
        void AttachOnCollision(std::function<void(const RigidBody2D& other)>&& func)
        {
            m_OnCollision = std::move(func);
        }
        void ClearCallbacks();

        void IsFixed(bool val) { m_IsFixed = val; }
        bool IsFixed() const { return m_IsFixed; }

        const Vector2<float>& Position() const { return m_Position; }
        void                  Position(const Vector2<float>& val)
        {
            m_Position = val;
            if (m_HitShape)
                m_HitShape->Origin(m_HitShape->Origin() + m_Position);
            if (m_OnPosChange)
                m_OnPosChange(m_Position);
        }

        const Vector2<float>& Velocity() const { return m_Velocity; }
        void                  Velocity(const Vector2<float>& val) { m_Velocity = val; }

        void HitShape(std::unique_ptr<Physics::HitShape> pVal) { m_HitShape = std::move(pVal); }
        const std::unique_ptr<Physics::HitShape>& HitShape() const { return m_HitShape; }

      private:
        void CollisionOccurred(const RigidBody2D& other);
    };
} // namespace Ignosi::Modules::Physics
