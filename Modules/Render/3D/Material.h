#pragma once

#include "../Texture/Texture.h"

#include <LinearAlgebra/Vector3.hpp>

#include <memory>

namespace nate::Modules::Render
{
    struct Material
    {
        std::shared_ptr<Texture> Diffuse;
        std::shared_ptr<Texture> Specular;
        std::shared_ptr<Texture> Normal;
        std::shared_ptr<Texture> Height;

        float Shininess;
    };
} // namespace nate::Modules::Render
