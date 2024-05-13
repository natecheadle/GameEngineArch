#pragma once

#include "VertexDataConfig.h"

#include <LinearAlgebra/Vector2.hpp>
#include <LinearAlgebra/Vector3.hpp>

namespace Ignosi::Libraries::Renderer
{
    struct SpriteVertexData
    {
        Math::Vector2<float> Position;
        Math::Vector2<float> TextureCoord;

        static VertexDataConfig describe()
        {
            static VertexDataConfig config;
            if (config.Describe().empty())
            {
                config.PushBackConfig<float, 2>();
                config.PushBackConfig<float, 2>();
            }
            return config;
        }
    };

    static_assert(sizeof(SpriteVertexData) == sizeof(float) * 4, "SpriteVertexData must be tightly packed");
} // namespace Ignosi::Libraries::Renderer
