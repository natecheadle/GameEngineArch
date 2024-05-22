
#pragma once

#include "IMaterial.h"
#include "ITexture.h"

namespace Ignosi::Libraries::Renderer
{
    class Material : public IMaterial
    {
        ITexture* m_Diffuse{nullptr};
        ITexture* m_Specular{nullptr};
        ITexture* m_Normal{nullptr};
        ITexture* m_Height{nullptr};

        float m_Shininess{0.0};

      public:
        Material() = default;
        Material(ITexture* diffuse, ITexture* specular, ITexture* normal, ITexture* height, float shininess);

        ~Material() override = default;

        Material(Material&&)      = default;
        Material(const Material&) = default;

        Material& operator=(Material&&)      = default;
        Material& operator=(const Material&) = default;

        ITexture* Diffuse() const override { return m_Diffuse; }
        ITexture* Specular() const override { return m_Specular; }
        ITexture* Normal() const override { return m_Normal; }
        ITexture* Height() const override { return m_Height; }
        float     Shininess() const override { return m_Shininess; }

        void Diffuse(ITexture* value) override { m_Diffuse = value; }
        void Specular(ITexture* value) override { m_Specular = value; }
        void Normal(ITexture* value) override { m_Normal = value; }
        void Height(ITexture* value) override { m_Height = value; }
        void Shininess(float value) override { m_Shininess = value; }
    };
} // namespace Ignosi::Libraries::Renderer
