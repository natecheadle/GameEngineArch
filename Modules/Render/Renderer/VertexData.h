#pragma once

#include <Vector3.hpp>

namespace nate::Modules::Render
{

    struct VertexData
    {
        Vector3<float> Position;
        Vector3<float> Color;
        Vector2<float> TextureCoord;
    };
} // namespace nate::Modules::Render
