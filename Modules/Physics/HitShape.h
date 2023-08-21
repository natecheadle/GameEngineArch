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

        virtual std::array<Vector2<float>, 2> ProjectShape(const Vector2<float>& axis) const;

      protected:
        HitShape(Render::Renderer* pRenderer);

        Render::Sprite CreateDebugSprite() const;

        virtual std::span<const float> VertexData() const = 0;
        virtual Vector2<float>         Scale() const      = 0;

        const Render::VertexDataConfig& VertexConfig() const { return m_VertexConfig; }
        Render::Renderer*               Renderer() const { return m_pRenderer; }
    };
} // namespace Ignosi::Modules::Physics
