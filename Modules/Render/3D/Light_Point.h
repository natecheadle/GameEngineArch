#pragma once

#include "Light.h"
#include "Light_Attenuation.h"

namespace Ignosi::Modules::Render
{
    struct Light_Point
    {
        Vector3<float>    Position;
        Light_Attenuation Attenuation;
        Light             Light;
    };
} // namespace Ignosi::Modules::Render
