#pragma once

#include "3D/Mesh3D.h"

#include <3D/Sprite.h>
#include <LinearAlgebra/Vector2.hpp>
#include <Renderer/VertexDataConfig.h>

namespace Ignosi::Modules::Physics
{
    class HitShape
    {
        Vector2<float>           m_Origin;
        bool                     m_IsDebugModeEnabled;
        Render::VertexDataConfig m_VertexConfig;
        Render::Renderer*        m_pRenderer;

      public:
        virtual ~HitShape() = default;

        const Vector2<float>& Origin() const { return m_Origin; }
        void                  Origin(const Vector2<float>& value) { m_Origin = value; }

        bool IsDebugModeEnabled() const { return m_IsDebugModeEnabled; }
        void EnableDebugMode();
        void DisableDebugMode();

      protected:
        HitShape(Render::Renderer* pRenderer);

        virtual Render::Sprite                     CreateDebugSprite() const;
        virtual const std::vector<Vector2<float>>& GetVertexData() const = 0;

        const Render::VertexDataConfig& VertexConfig() const { return m_VertexConfig; }
        Render::Renderer*               Renderer() const { return m_pRenderer; }
    };
} // namespace Ignosi::Modules::Physics
