#pragma once

#include "Light.h"
#include "Light_Attenuation.h"

namespace Ignosi::Libraries::Renderer
{
    struct Light_Point
    {
        Math::Vector3<float> Position;
        Light_Attenuation    Attenuation;
        struct Light         Light;
    };
} // namespace Ignosi::Libraries::Renderer
