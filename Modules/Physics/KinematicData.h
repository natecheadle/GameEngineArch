#pragma once

#include "Units/Angle.hpp"
#include "Units/AngularAcceleration.hpp"
#include "Units/AngularVelocity.hpp"

#include <IComponent.h>
#include <LinearAlgebra/Vector3.hpp>

namespace Ignosi::Modules::Physics
{
    class KinematicData final : ECS::IComponent
    {
      public:
        struct Data
        {
            Vector3<float>         Position;
            Vector3<Radian<float>> Angle;

            Vector3<float>                  Velocity;
            Vector3<RadianPerSecond<float>> AngularVelocity;

            Vector3<float>                           Acceleration;
            Vector3<RadianPerSecondPerSecond<float>> AngularAcceleration;
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

        const ECS::Tag& Tag() const override
        {
            static const ECS::Tag s_tag = ECS::Tag::Create("KinematicData");
            return s_tag;
        }

        const Vector3<float>&         Position() const { return m_Data.Position; }
        const Vector3<Radian<float>>& Angle() const { return m_Data.Angle; }

        const Vector3<float>&                  Velocity() const { return m_Data.Velocity; }
        const Vector3<RadianPerSecond<float>>& AngularVelocity() const { return m_Data.AngularVelocity; }

        const Vector3<float>&                           Acceleration() const { return m_Data.Acceleration; }
        const Vector3<RadianPerSecondPerSecond<float>>& AngularAcceleration() const { return m_Data.AngularAcceleration; }

        void Position(Vector3<float> value) { m_Data.Position = std::move(value); }
        void Angle(Vector3<Radian<float>> value) { m_Data.Angle = std::move(value); }

        void Velocity(Vector3<float> value) { m_Data.Velocity = std::move(value); }
        void AngularVelocity(Vector3<RadianPerSecond<float>> value) { m_Data.AngularVelocity = std::move(value); }

        void Acceleration(Vector3<float> value) { m_Data.Acceleration = std::move(value); }
        void AngularAcceleration(RadianPerSecondPerSecond<float> value) { m_Data.AngularAcceleration = std::move(value); }

        void Update(double dt);

      private:
        Data m_Data;
    };
} // namespace Ignosi::Modules::Physics
