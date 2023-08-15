#pragma once

#include "HitShape.h"
#include "LinearAlgebra/Vector2.hpp"
#include "Units/Radian.hpp"

#include <vector>

namespace Ignosi::Modules::Physics
{
    class HitRectangle : public HitShape
    {
        float                       m_Width{1.0f};
        float                       m_Height{1.0f};
        std::vector<Vector2<float>> m_TestAxes;
        std::vector<Vector2<float>> m_Corners;

      public:
        HitRectangle(Render::Renderer* pRenderer);
        ~HitRectangle() override = default;

        float Width() const { return m_Width; }
        void  Width(float value);
        float Height() const { return m_Height; }
        void  Height(float value);

        void Rotation(const Radian<float>& value) override;

        const std::vector<Vector2<float>>& TestAxes(const HitShape& /*other*/) override { return m_TestAxes; }
        const std::vector<Vector2<float>>& Corners() const override { return m_Corners; }
        std::array<Vector2<float>, 2>      ProjectShape(const Vector2<float>& other) const override;

      protected:
        std::span<const float> VertexData() const override;
        Vector2<float>         Scale() const override { return {m_Width, m_Height}; }

      private:
        void UpdatePrivateVectors();
    };
} // namespace Ignosi::Modules::Physics
