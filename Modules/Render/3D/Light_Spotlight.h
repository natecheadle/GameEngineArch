#pragma once

#include "Light_Point.h"

#include <Units/Radian.hpp>

namespace nate::Modules::Render
{
    struct Light_Spotlight : public Light_Point
    {
        Vector3<float> Direction;
        Radian<float>  Cutoff;
        Radian<float>  OuterCutoff;
    };
} // namespace nate::Modules::Render
