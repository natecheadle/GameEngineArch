#pragma once

#include "HitShape.h"
#include "LinearAlgebra/Vector2.hpp"

#include <vector>

namespace Ignosi::Modules::Physics
{
    class HitRectangle : public HitShape
    {
        float                       m_Width;
        float                       m_Height;
        std::vector<Vector2<float>> m_Vertexes;

      public:
        HitRectangle(Render::Renderer* pRenderer);

        float Width() const { return m_Width; }
        void  Width(float value);

        float Height() const { return m_Height; }
        void  Height(float value);

      protected:
        const std::vector<Vector2<float>>& GetVertexData() const override { return m_Vertexes; }

      private:
        void ResetVertexData();
    };
} // namespace Ignosi::Modules::Physics
