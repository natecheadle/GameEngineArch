#pragma once

#include "RGB_Color.h"

#include <Vector3.hpp>

namespace nate::Modules::Render
{
    struct Light
    {
        RGB_Color Ambient;
        RGB_Color Diffuse;
        RGB_Color Specular;
    };
} // namespace nate::Modules::Render
