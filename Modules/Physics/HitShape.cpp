#include "HitShape.h"

#include "LinearAlgebra/Vector2.hpp"

namespace Ignosi::Modules::Physics
{
    HitShape::HitShape(Render::Renderer* pRenderer)
        : m_pRenderer(pRenderer)
    {
        m_VertexConfig.PushBackConfig(Vector2<float>());
    }

    void HitShape::EnableDebugMode()
    {
        m_IsDebugModeEnabled = true;
    }

    void HitShape::DisableDebugMode()
    {
        m_IsDebugModeEnabled = false;
    }

    Render::Sprite HitShape::CreateDebugSprite() const
    {
        assert(Renderer());

        Render::Sprite rslt(Renderer(), VertexConfig(), VertexData());
        rslt.Size(Scale());
        rslt.Rotation(m_Rotation);
        return std::move(rslt);
    }

    std::array<Vector2<float>, 2> HitShape::ProjectShape(const Vector2<float>& axis) const
    {
        const std::vector<Vector2<float>>& corners = Corners();
        std::array<Vector2<float>, 2>      points;

        Vector2<float> axis_perp = axis.perp();
        for (size_t i = 0; i < corners.size(); ++i)
        {
            float x = 0.0;
            float y = 0.0;
            if (axis.x() != 0.0 && axis.y() != 0.0)
            {
                float m  = axis.y() / axis.x();
                float m2 = axis_perp.y() / axis_perp.x();

                x = (-m2 * corners[i][0] + corners[i][1]) / m - m2;
                y = m * x;
            }
            else if (axis.x() == 0.0)
            {
                y = corners[i][1];
            }
            else
            {
                x = corners[i][0];
            }

            if (i == 0)
            {
                points[0].x(x);
                points[0].y(y);

                points[1] = points[0];
            }
            else
            {
                if (std::abs(axis.x()) > std::abs(axis.y()))
                {
                    if (x < points[0].x())
                    {
                        points[0].x(x);
                        points[0].y(y);
                    }
                    else if (x > points[0].x())
                    {
                        points[1].x(x);
                        points[1].y(y);
                    }
                }
                else
                {
                    if (y < points[0].y())
                    {
                        points[0].x(x);
                        points[0].y(y);
                    }
                    else if (y > points[0].y())
                    {
                        points[1].x(x);
                        points[1].y(y);
                    }
                }
            }
        }

        return points;
    }
} // namespace Ignosi::Modules::Physics
