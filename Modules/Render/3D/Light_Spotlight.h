#pragma once

#include "Light.h"
#include "Light_Attenuation.h"

#include <Units/Radian.hpp>

namespace nate::Modules::Render
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
} // namespace nate::Modules::Render
