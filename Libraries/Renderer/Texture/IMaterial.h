#pragma once

#include "ITexture.h"

namespace Ignosi::Libraries::Renderer
{
    class IMaterial
    {
      public:
        IMaterial()  = default;
        ~IMaterial() = default;

        IMaterial(IMaterial&&)      = default;
        IMaterial(const IMaterial&) = default;

        IMaterial& operator=(IMaterial&&)      = default;
        IMaterial& operator=(const IMaterial&) = default;

        virtual ITexture* Diffuse() const   = 0;
        virtual ITexture* Specular() const  = 0;
        virtual ITexture* Normal() const    = 0;
        virtual ITexture* Height() const    = 0;
        virtual float     Shininess() const = 0;
    };
} // namespace Ignosi::Libraries::Renderer
