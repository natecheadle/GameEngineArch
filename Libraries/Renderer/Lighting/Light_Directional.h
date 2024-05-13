#pragma once

#include "Light.h"

namespace Ignosi::Libraries::Renderer
{
    struct Light_Directional
    {
        Math::Vector3<float> Direction;
        struct Light         Light;
    };
} // namespace Ignosi::Libraries::Renderer
