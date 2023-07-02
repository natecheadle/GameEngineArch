#pragma once

#include "RGB_Color.h"

#include <LinearAlgebra/Vector3.hpp>

namespace Ignosi::Modules::Render
{
    struct Light
    {
        RGB_Color Ambient;
        RGB_Color Diffuse;
        RGB_Color Specular;
    };
} // namespace Ignosi::Modules::Render
