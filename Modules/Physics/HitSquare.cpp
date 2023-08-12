#include "HitSquare.h"

#include <3D/GenericVertexes.hpp>
#include <LinearAlgebra/SquareMatrix4x4.hpp>
#include <LinearAlgebra/Vector3.hpp>
#include <LinearAlgebra/Vector4.hpp>

namespace Ignosi::Modules::Physics
{
    namespace
    {
        const float* rect_begin = Render::RectangleVertexes.data()->data();
        const size_t rect_size  = sizeof(Render::RectangleVertexes) / sizeof(float);
        const float* rect_end   = rect_begin + rect_size;
    } // namespace

    HitSquare::HitSquare(Render::Renderer* pRenderer)
        : HitShape(pRenderer)
        , m_Vertexes(rect_begin, rect_end)
    {
    }

    void HitSquare::Size(float value)
    {
        m_Size = value;
        ResetVertexData();
    }

    void HitSquare::ResetVertexData()
    {
        SquareMatrix4x4<float> scale = SquareMatrix4x4<float>::scale_init({m_Size, m_Size});
        for (size_t i = 0; i < m_Vertexes.size(); ++i)
        {
            m_Vertexes[i] = Render::RectangleVertexes[i];

            Vector3<float> temp = (scale * Vector4<float>(m_Vertexes[i])).ToVector3();
            m_Vertexes[i].x(temp.x());
            m_Vertexes[i].y(temp.y());
        }
    }
} // namespace Ignosi::Modules::Physics
