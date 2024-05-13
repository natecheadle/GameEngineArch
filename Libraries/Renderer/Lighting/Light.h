#pragma once

#include "Texture/RGB_Color.h"

#include <LinearAlgebra/Vector3.hpp>

namespace Ignosi::Libraries::Renderer
{
    struct Light
    {
        RGB_Color Ambient;
        RGB_Color Diffuse;
        RGB_Color Specular;
    };
} // namespace Ignosi::Modules::Render
