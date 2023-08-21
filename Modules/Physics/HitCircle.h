#pragma once

#include "HitShape.h"

#include <3D/Sprite.h>
#include <LinearAlgebra/Vector2.hpp>
#include <Units/Degree.hpp>

#include <vector>

namespace Ignosi::Modules::Physics
{
    class HitCircle : public HitShape
    {
        float m_Radius;

        std::vector<Vector2<float>> m_TestAxes;
        std::vector<Vector2<float>> m_Corners;

      public:
        HitCircle(Render::Renderer* pRenderer);
        ~HitCircle() override = default;

        float Radius() const override { return m_Radius; }
        void  Radius(float value) { m_Radius = value; }

        const std::vector<Vector2<float>>& Corners() const override { return m_Corners; }
        const std::vector<Vector2<float>>& TestAxes() const override { return m_TestAxes; }

        std::array<Vector2<float>, 2> ProjectShape(const Vector2<float>& axis) const override;

      protected:
        std::span<const float> VertexData() const override;
        Vector2<float>         Scale() const override { return {m_Radius, m_Radius}; }
    };
} // namespace Ignosi::Modules::Physics
