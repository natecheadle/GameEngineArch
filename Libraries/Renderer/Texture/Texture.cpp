#include "Texture.h"

namespace Ignosi::Libraries::Renderer
{
    Texture::Texture(enum TextureUnit unit)
        : m_Unit(unit)
    {
    }

    TextureUnit Texture::TextureUnit() const
    {
        return m_Unit;
    }

} // namespace Ignosi::Libraries::Renderer
