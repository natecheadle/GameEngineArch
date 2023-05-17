#pragma once

namespace nate::Modules::Render
{
    struct Light_Attenuation
    {
        float Constant;
        float Linear;
        float Quadratic;
    };
} // namespace nate::Modules::Render
