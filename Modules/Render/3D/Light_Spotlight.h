#pragma once

#include "Light.h"
#include "Light_Attenuation.h"

#include <Units/Radian.hpp>

namespace Ignosi::Modules::Render
{
    struct Light_Spotlight
    {
        Vector3<float>    Direction;
        Vector3<float>    Position;
        Radian<float>     Cutoff;
        Radian<float>     OuterCutoff;
        Light_Attenuation Attenuation;
        Light             Light;
    };
} // namespace Ignosi::Modules::Render
