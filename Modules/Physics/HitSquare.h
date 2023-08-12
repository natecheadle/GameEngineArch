#pragma once

#include "HitShape.h"
#include "LinearAlgebra/Vector2.hpp"

#include <vector>

namespace Ignosi::Modules::Physics
{
    class HitSquare : public HitShape
    {
        float                       m_Size;
        std::vector<Vector2<float>> m_Vertexes;

      public:
        HitSquare(Render::Renderer* pRenderer);

        float Size() const { return m_Size; }
        void  Size(float value);

      protected:
        const std::vector<Vector2<float>>& GetVertexData() const override { return m_Vertexes; }

      private:
        void ResetVertexData();
    };
} // namespace Ignosi::Modules::Physics
