#include "Material.h"

namespace Ignosi::Libraries::Renderer
{
    Material::Material(ITexture* diffuse, ITexture* specular, ITexture* normal, ITexture* height, float shininess)
        : m_Diffuse(diffuse)
        , m_Specular(specular)
        , m_Normal(normal)
        , m_Height(height)
        , m_Shininess(shininess)
    {
    }
} // namespace Ignosi::Libraries::Renderer
