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

        friend PhysicsSystem;

      public:
        void Update(float dt);

        void IsFixed(bool val) { m_IsFixed = val; }
        bool IsFixed() const { return m_IsFixed; }

        const Vector2<float>& Position() const { return m_Position; }
        Vector2<float>&       Position() { return m_Position; }
        void                  Position(const Vector2<float>& val) { m_Position = val; }

        const Vector2<float>& Velocity() const { return m_Velocity; }
        void                  Velocity(const Vector2<float>& val) { m_Velocity = val; }

        const Vector2<float>& HitBox() const { return m_HitBox; }
        void                  HitBox(const Vector2<float>& val) { m_HitBox = val; }

      private:
        void CollisionOccurred(const RigidBody2D& other);
    };
} // namespace nate::Modules::Physics
