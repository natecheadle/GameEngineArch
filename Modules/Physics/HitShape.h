#pragma once

#include "DebugSpriteEntity.h"

#include <LinearAlgebra/Vector2.hpp>

namespace Ignosi::Modules::Physics
{
    class HitShape
    {
        Vector2<float> m_Origin;
        bool           m_IsDebugModeEnabled;

      public:
        virtual ~HitShape() = default;

        const Vector2<float>& Origin() const { return m_Origin; }
        void                  Origin(const Vector2<float>& value) { m_Origin = value; }

        bool IsDebugModeEnabled() const { return m_IsDebugModeEnabled; }
        void EnableDebugMode();
        void DisableDebugMode();

      protected:
        virtual std::unique_ptr<DebugSpriteEntity> CreateDebugSprite() const = 0;
    };
} // namespace Ignosi::Modules::Physics
