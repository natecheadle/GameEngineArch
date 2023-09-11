#pragma once

#include "ComponentPointer.h"
#include "HitShape.h"
#include "KinematicData.h"
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
        float                       m_Radius{1.0f};

      public:
        HitRectangle(ECS::WeakComponentPointer<KinematicData> pPosition);
        ~HitRectangle() override = default;

        float Radius() const override { return m_Radius; }
        float Width() const { return m_Width; }
        void  Width(float value);
        float Height() const { return m_Height; }
        void  Height(float value);

        const std::vector<Vector2<float>>& Corners() override;
        const std::vector<Vector2<float>>& TestAxes() override;

      private:
        void UpdatePrivateVectors();
    };
} // namespace Ignosi::Modules::Physics
