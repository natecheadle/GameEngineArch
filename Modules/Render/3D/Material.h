#pragma once

#include "../Texture/Texture.h"

#include <Vector3.hpp>

#include <memory>

namespace nate::Modules::Render
{
    struct Material
    {
        std::shared_ptr<Texture> Diffuse;
        std::shared_ptr<Texture> Specular;
        // std::vector<std::shared_ptr<Texture>> Normal;
        // std::vector<std::shared_ptr<Texture>> Height;

        float Shininess;
    };
} // namespace nate::Modules::Render
