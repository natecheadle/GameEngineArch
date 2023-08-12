#include "HitRectangle.h"

#include <3D/GenericVertexes.hpp>

namespace Ignosi::Modules::Physics
{
    namespace
    {
        const float* rect_begin = Render::RectangleVertexes.data()->data();
        const size_t rect_size  = sizeof(Render::RectangleVertexes) / sizeof(float);
        const float* rect_end   = rect_begin + rect_size;
    } // namespace

    HitRectangle::HitRectangle(Render::Renderer* pRenderer)
        : HitShape(pRenderer)
        , m_Vertexes(rect_begin, rect_end)
    {
    }

    void HitRectangle::Width(float value)
    {
        m_Width = value;
        ResetVertexData();
    }

    void HitRectangle::Height(float value)
    {
        m_Height = value;
        ResetVertexData();
    }

    void HitRectangle::ResetVertexData()
    {
        SquareMatrix4x4<float> scale = SquareMatrix4x4<float>::scale_init({m_Width, m_Height});
        for (size_t i = 0; i < m_Vertexes.size(); ++i)
        {
            m_Vertexes[i] = Render::RectangleVertexes[i];

            Vector3<float> temp = (scale * Vector4<float>(m_Vertexes[i])).ToVector3();
            m_Vertexes[i].x(temp.x());
            m_Vertexes[i].y(temp.y());
        }
    }
} // namespace Ignosi::Modules::Physics
