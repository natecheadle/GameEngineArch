
#pragma once

#include "ITexture.h"
#include "Texture/Texture.h"

#include <memory>

namespace Ignosi::Libraries::Renderer
{
    class Material : public IMaterial
    {
        std::shared_ptr<Texture> m_Diffuse;
        std::shared_ptr<Texture> m_Specular;
        std::shared_ptr<Texture> m_Normal;
        std::shared_ptr<Texture> m_Height;

        float m_Shininess;

      public:
        Material()           = default;
        ~Material() override = default;

        Material(Material&&)      = default;
        Material(const Material&) = default;

        Material& operator=(Material&&)      = default;
        Material& operator=(const Material&) = default;

        ITexture* Diffuse() const override { return m_Diffuse.get(); }
        ITexture* Specular() const override { return m_Specular.get(); }
        ITexture* Normal() const override { return m_Normal.get(); }
        ITexture* Height() const override { return m_Height.get(); }
        float     Shininess() const override { return m_Shininess.get(); }
    };
} // namespace Ignosi::Libraries::Renderer
