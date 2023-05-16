#pragma once

#include "Light.h"

namespace nate::Modules::Render
{
    struct Light_Point : public Light
    {
        Vector3<float> Position;

        struct
        {
            float Constant;
            float Linear;
            float Quadratic;
        } Attenuation;
    };
} // namespace nate::Modules::Render
