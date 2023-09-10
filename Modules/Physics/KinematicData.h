#pragma once

#include "Units/AngularAcceleration.hpp"
#include "Units/AngularVelocity.hpp"
#include "Units/Radian.hpp"

#include <LinearAlgebra/Vector3.hpp>

namespace Ignosi::Modules::Physics
{
    class KinematicData final
    {
      public:
        struct Data
        {
            Vector3<float> Position;
            Radian<float>  Angle;

            Vector3<float>         Velocity;
            RadianPerSecond<float> AngularVelocity;

            Vector3<float>                  Acceleration;
            RadianPerSecondPerSecond<float> AngularAcceleration;
        };

        KinematicData() = default;
        KinematicData(Data data)
            : m_Data(std::move(data))
        {
        }

        ~KinematicData() = default;

        KinematicData(const KinematicData& other)     = default;
        KinematicData(KinematicData&& other) noexcept = default;

        KinematicData& operator=(const KinematicData& other) = default;
        KinematicData& operator=(KinematicData&& other)      = default;

        const Vector3<float>& Position() const { return m_Data.Position; }
        const Radian<float>&  Angle() const { return m_Data.Angle; }

        const Vector3<float>&         Velocity() const { return m_Data.Velocity; }
        const RadianPerSecond<float>& AnglularVelocity() const { return m_Data.AngularVelocity; }

        const Vector3<float>&                  Acceleration() const { return m_Data.Acceleration; }
        const RadianPerSecondPerSecond<float>& AngularAcceleration() const { return m_Data.AngularAcceleration; }

        void Position(Vector3<float> value) { m_Data.Position = std::move(value); }
        void Angle(const Radian<float>& value) { m_Data.Angle = value; }

        void Velocity(Vector3<float> value) { m_Data.Velocity = std::move(value); }
        void AngularVelocity(const RadianPerSecond<float>& value) { m_Data.AngularVelocity = value; }

        void Acceleration(Vector3<float> value) { m_Data.Acceleration = std::move(value); }
        void AngularAcceleration(const RadianPerSecondPerSecond<float>& value) { m_Data.AngularAcceleration = value; }

        void Update(double dt);

      private:
        Data m_Data;
    };
} // namespace Ignosi::Modules::Physics
