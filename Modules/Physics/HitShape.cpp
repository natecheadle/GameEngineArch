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
} // namespace Ignosi::Modules::Physics
