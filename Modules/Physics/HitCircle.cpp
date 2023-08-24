#include "HitCircle.h"

#include "LinearAlgebra/Vector2.hpp"

#include <3D/GenericVertexes.hpp>
#include <LinearAlgebra/SquareMatrix4x4.hpp>

#include <vector>

namespace Ignosi::Modules::Physics
{
    namespace
    {
        constexpr size_t VertCount    = 16;
        constexpr auto   CIRCLE_VERTS = Render::GenerateCircleVertexes<VertCount>();
        const float*     circle_begin = CIRCLE_VERTS.data()->data();
        const size_t     circle_size  = sizeof(CIRCLE_VERTS) / sizeof(float);
        const float*     circle_end   = circle_begin + circle_size;
    } // namespace

    HitCircle::HitCircle(Render::Renderer* pRenderer)
        : HitShape(pRenderer)
    {
    }

    std::span<const float> HitCircle::VertexData() const
    {
        return std::span<const float>(circle_begin, circle_end);
    }

    std::array<Vector2<float>, 2> HitCircle::ProjectShape(const Vector2<float>& axis) const
    {
        Vector2<float>                origin = Origin();
        std::array<Vector2<float>, 2> rslt;
        rslt[0] = origin;
        rslt[1] = origin;

        if (axis[0] != 0.0 && axis[1] != 0.0)
        {
            Degree<float> m(axis[1] / axis[0]);
            float         x = origin[0] + cos(m) * m_Radius;
            float         y = origin[1] + sin(m) + m_Radius;

            rslt[0][0] += x;
            rslt[0][1] += y;

            rslt[1][0] -= x;
            rslt[1][1] -= y;
        }
        else if (axis[0] == 0.0)
        {
            rslt[1][0] = 0.0;
            rslt[0][0] = 0.0;

            rslt[1][1] += m_Radius;
            rslt[0][1] -= m_Radius;
        }
        else
        {
            rslt[1][0] += m_Radius;
            rslt[0][0] -= m_Radius;

            rslt[1][1] = 0.0;
            rslt[0][1] = 0.0;
        }

        return rslt;
    }
} // namespace Ignosi::Modules::Physics
