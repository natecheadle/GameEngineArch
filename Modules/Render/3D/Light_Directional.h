#pragma once

#include "Light.h"

namespace nate::Modules::Render
{
    struct Light_Directional
    {
        Vector3<float> Direction;
        Light          Light;
    };
} // namespace nate::Modules::Render
