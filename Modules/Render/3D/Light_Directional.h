#pragma once

#include "Light.h"

namespace Ignosi::Modules::Render
{
    struct Light_Directional
    {
        Vector3<float> Direction;
        Light          Light;
    };
} // namespace Ignosi::Modules::Render
