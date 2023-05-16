#pragma once

#include "Light.h"

#include <Units/Radian.hpp>

namespace nate::Modules::Render
{
    struct Light_Spotlight : public Light
    {

        Vector3<float> Position;
        Vector3<float> Direction;
        Radian<float>  Cutoff;
        Radian<float>  OuterCutoff;
    };
} // namespace nate::Modules::Render
