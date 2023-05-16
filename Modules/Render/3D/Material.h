#pragma once

#include "../Texture/Texture.h"

#include <Vector3.hpp>

namespace nate::Modules::Render
{
    struct Material
    {
        std::shared_ptr<Texture> Diffuse;
        std::shared_ptr<Texture> Specular;

        float Shininess;
    };
} // namespace nate::Modules::Render
