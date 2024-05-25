#pragma once

#include "VertexDataConfig.h"

#include <LinearAlgebra/Vector.hpp>

namespace Ignosi::Libraries::Renderer
{
    struct VertexData
    {
        Math::Vector3<float> Position;
        Math::Vector3<float> Normal;
        Math::Vector2<float> TextureCoord;
        Math::Vector3<float> TextureTangent;
        Math::Vector3<float> TextureBitangent;

        static VertexDataConfig describe()
        {
            static VertexDataConfig config;
            if (config.Describe().empty())
            {
                config.PushBackConfig<float, 3>();
                config.PushBackConfig<float, 3>();
                config.PushBackConfig<float, 2>();
                config.PushBackConfig<float, 3>();
                config.PushBackConfig<float, 3>();
            }
            return config;
        }
    };

    static_assert(sizeof(VertexData) == sizeof(float) * 14, "VertexData must be tightly packed");
} // namespace Ignosi::Libraries::Renderer
