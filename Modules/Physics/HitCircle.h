#pragma once

#include "3D/Sprite.h"
#include "HitShape.h"
#include "LinearAlgebra/Vector2.hpp"

#include <vector>

namespace Ignosi::Modules::Physics
{
    class HitCircle : public HitShape
    {
        float                       m_Radius;
        float                       m_VertCount;
        std::vector<Vector2<float>> m_Vertexes;

      public:
        HitCircle(Render::Renderer* pRenderer);

        float Radius() const { return m_Radius; }
        void  Radius(float value);

      protected:
        const std::vector<Vector2<float>>& GetVertexData() const override { return m_Vertexes; }

      private:
        void ResetVertexData();
    };
} // namespace Ignosi::Modules::Physics
