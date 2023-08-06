#pragma once

#include "HitShape.h"

#include <LinearAlgebra/Vector2.hpp>

#include <functional>
#include <vector>

namespace Ignosi::Modules::Physics
{
    class PhysicsSystem;

    class RigidBody2D
    {
        Vector2<float>                         m_Position;
        Vector2<float>                         m_Velocity;
        std::vector<std::unique_ptr<HitShape>> m_HitBoxes;
        bool                                   m_IsFixed{true};

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
            if (m_OnPosChange)
                m_OnPosChange(m_Position);
        }

        const Vector2<float>& Velocity() const { return m_Velocity; }
        void                  Velocity(const Vector2<float>& val) { m_Velocity = val; }

        const std::vector<std::unique_ptr<HitShape>>& HitBoxes() const { return m_HitBoxes; }
        void HitBoxes(std::vector<std::unique_ptr<HitShape>> val) { m_HitBoxes = std::move(val); }

      private:
        void CollisionOccurred(const RigidBody2D& other);
    };
} // namespace Ignosi::Modules::Physics
