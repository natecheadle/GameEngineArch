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
        size_t data_size = GetVertexData().size() * 2;
        return Render::Sprite(
            Renderer(),
            VertexConfig(),
            {GetVertexData().data()->data(), GetVertexData().data()->data() + data_size});
    }
} // namespace Ignosi::Modules::Physics
