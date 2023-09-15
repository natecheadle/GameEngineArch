#pragma once

#include "HitShape.h"
#include "KinematicData.h"

#include <LinearAlgebra/Vector2.hpp>
#include <Units/Degree.hpp>

#include <vector>

namespace Ignosi::Modules::Physics
{
    class HitCircle : public HitShape
    {
        float m_Radius{1.0};

        std::vector<Vector2<float>> m_TestAxes;
        std::vector<Vector2<float>> m_Corners;

      public:
        HitCircle(ECS::WeakComponentPointer<KinematicData> pPosition);
        HitCircle(ECS::WeakComponentPointer<KinematicData> pPosition, float radius);

        ~HitCircle() override = default;

        float Radius() const override { return m_Radius; }
        void  Radius(float value) { m_Radius = value; }

        const std::vector<Vector2<float>>& Corners() override { return m_Corners; }
        const std::vector<Vector2<float>>& TestAxes() override { return m_TestAxes; }

        std::array<Vector2<float>, 2> ProjectShape(const Vector2<float>& axis) override;
    };
} // namespace Ignosi::Modules::Physics
