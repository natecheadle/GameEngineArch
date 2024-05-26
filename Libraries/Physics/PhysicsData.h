#pragma once

#include <LinearAlgebra/Vector.hpp>

namespace Ignosi::Libraries::Physics
{
    class PhysicsData
    {
        Math::Vector3<double> m_Position;
        Math::Vector3<double> m_Velocity;
        Math::Vector3<double> m_Acceleration;

      public:
        Math::Vector3<double> Position() const { return m_Position; }
        Math::Vector3<double> Velocity() const { return m_Velocity; }
        Math::Vector3<double> Acceleration() const { return m_Acceleration; }

        void Position(const Math::Vector3<double>& value) { m_Position = value; }
        void Velocity(const Math::Vector3<double>& value) { m_Velocity = value; }
        void Acceleration(const Math::Vector3<double>& value) { m_Acceleration = value; }
    };
} // namespace Ignosi::Libraries::Physics
