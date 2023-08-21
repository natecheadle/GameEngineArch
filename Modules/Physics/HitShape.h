#pragma once

#include <3D/Sprite.h>
#include <LinearAlgebra/SquareMatrix4x4.hpp>
#include <LinearAlgebra/Vector2.hpp>
#include <Renderer/VertexDataConfig.h>
#include <Units/Radian.hpp>

#include <array>
#include <vector>

namespace Ignosi::Modules::Physics
{
    class HitShape
    {
        Vector2<float>           m_Origin;
        Radian<float>            m_Rotation;
        bool                     m_IsDebugModeEnabled;
        Render::VertexDataConfig m_VertexConfig;
        Render::Renderer*        m_pRenderer;

      public:
        virtual ~HitShape() = default;

        const Radian<float>& Rotation() const { return m_Rotation; }
        virtual void         Rotation(const Radian<float>& value) { m_Rotation = value; }

        const Vector2<float>& Origin() const { return m_Origin; }
        void                  Origin(const Vector2<float>& value) { m_Origin = value; }

        bool IsDebugModeEnabled() const { return m_IsDebugModeEnabled; }
        void EnableDebugMode();
        void DisableDebugMode();

        virtual float                              Radius() const   = 0;
        virtual const std::vector<Vector2<float>>& Corners() const  = 0;
        virtual const std::vector<Vector2<float>>& TestAxes() const = 0;

        virtual std::array<Vector2<float>, 2> ProjectShape(const Vector2<float>& axis) const
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

      protected:
        HitShape(Render::Renderer* pRenderer);

        Render::Sprite CreateDebugSprite() const;

        virtual std::span<const float> VertexData() const = 0;
        virtual Vector2<float>         Scale() const      = 0;

        const Render::VertexDataConfig& VertexConfig() const { return m_VertexConfig; }
        Render::Renderer*               Renderer() const { return m_pRenderer; }
    };
} // namespace Ignosi::Modules::Physics
