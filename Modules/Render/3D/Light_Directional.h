#pragma once

#include "Light.h"

namespace nate::Modules::Render
{
    struct Light_Directional : public Light
    {
        Vector3<float> Direction;
    };
} // namespace nate::Modules::Render
