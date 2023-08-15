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
        m_TestAxes.resize(1);
    }

    const std::vector<Vector2<float>>& HitCircle::TestAxes(const HitShape& other)
    {
        // We are testing 2 circles return vector connecting their origins
        if (other.Corners().empty())
        {
            m_TestAxes[0] = (other.Origin() - Origin()).normalize_this();
        }
        else
        {

            const std::vector<Vector2<float>>& corners       = other.Corners();
            size_t                             closestCorner = 0;

            if (corners.size() > 1)
            {
                double closestDistance = corners[0].distance(Origin());
                for (size_t i = 1; i < corners.size(); ++i)
                {
                    double testDistance = corners[i].distance(Origin());
                    if (testDistance < closestDistance)
                    {
                        closestCorner   = i;
                        closestDistance = testDistance;
                    }
                }
            }

            m_TestAxes[0] = (corners[closestCorner] - Origin()).normalize_this();
        }

        return m_TestAxes;
    }

    std::array<Vector2<float>, 2> HitCircle::ProjectShape(const Vector2<float>& other) const
    {
        std::array<Vector2<float>, 2> rslt;
        rslt[0] = Origin() + (other * m_Radius);
        rslt[1] = Origin() - (other * m_Radius);

        return rslt;
    }

    std::span<const float> HitCircle::VertexData() const
    {
        return std::span<const float>(circle_begin, circle_end);
    }
} // namespace Ignosi::Modules::Physics
