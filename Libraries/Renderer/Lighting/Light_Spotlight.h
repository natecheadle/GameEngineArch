#pragma once

#include "Light.h"
#include "Light_Attenuation.h"

#include <Units/Radian.hpp>

namespace Ignosi::Libraries::Renderer
{
    struct Light_Spotlight
    {
        Math::Vector3<float> Direction;
        Math::Vector3<float> Position;
        Math::Radian<float>  Cutoff;
        Math::Radian<float>  OuterCutoff;
        Light_Attenuation    Attenuation;
        struct Light         Light;
    };
} // namespace Ignosi::Libraries::Renderer
