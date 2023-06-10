#pragma once

#include <LinearAlgebra/Vector2.hpp>

#include <functional>

namespace nate::Modules::Physics
{
    class PhysicsSystem;

    class RigidBody2D
    {
        Vector2<float> m_Position;
        Vector2<float> m_Velocity;
        Vector2<float> m_HitBox;
        bool           m_IsFixed;

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
        void IsFixed(bool val) { m_IsFixed = val; }
        bool IsFixed() const { return m_IsFixed; }

        const Vector2<float>& Position() const { return m_Position; }
        void                  Position(const Vector2<float>& val) { m_Position = val; }

        const Vector2<float>& Velocity() const { return m_Velocity; }
        void                  Velocity(const Vector2<float>& val) { m_Velocity = val; }

        const Vector2<float>& HitBox() const { return m_HitBox; }
        void                  HitBox(const Vector2<float>& val) { m_HitBox = val; }

      private:
        void CollisionOccurred(const RigidBody2D& other);
    };
} // namespace nate::Modules::Physics
