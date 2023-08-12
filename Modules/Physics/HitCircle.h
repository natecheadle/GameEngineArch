#pragma once

#include "HitShape.h"
#include <3D/

namespace Ignosi::Modules::Physics
{
    class HitCircle : public HitShape
    {
        float m_Radius;
        float m_VertCount;

      public:
        float Radius() const { return m_Radius; }
        void  Radius(float value) { m_Radius = value; }

      protected:
        std::unique_ptr<DebugSpriteEntity> CreateDebugSprite() const override;
    };
} // namespace Ignosi::Modules::Physics
