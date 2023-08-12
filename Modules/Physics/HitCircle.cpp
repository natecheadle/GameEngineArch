#include "HitCircle.h"

#include <3D/GenericVertexes.hpp>
#include <LinearAlgebra/SquareMatrix4x4.hpp>
#include <LinearAlgebra/Vector3.hpp>
#include <LinearAlgebra/Vector4.hpp>

namespace Ignosi::Modules::Physics
{
    namespace
    {
        constexpr auto CIRCLE_VERTS = Render::GenerateCircleVertexes<16>();
        const float*   circle_begin = CIRCLE_VERTS.data()->data();
        const size_t   circle_size  = sizeof(CIRCLE_VERTS) / sizeof(float);
        const float*   circle_end   = circle_begin + circle_size;
    } // namespace

    HitCircle::HitCircle(Render::Renderer* pRenderer)
        : HitShape(pRenderer)
        , m_Vertexes(circle_begin, circle_end)
    {
        ResetVertexData();
    }
    void HitCircle::Radius(float value)
    {
        m_Radius = value;
        ResetVertexData();
    }

    void HitCircle::ResetVertexData()
    {
        SquareMatrix4x4<float> scale = SquareMatrix4x4<float>::scale_init({m_Radius, m_Radius});
        for (size_t i = 0; i < m_Vertexes.size(); ++i)
        {
            m_Vertexes[i] = CIRCLE_VERTS[i];

            Vector3<float> temp = (scale * Vector4<float>(m_Vertexes[i])).ToVector3();
            m_Vertexes[i].x(temp.x());
            m_Vertexes[i].y(temp.y());
        }
    }
} // namespace Ignosi::Modules::Physics
