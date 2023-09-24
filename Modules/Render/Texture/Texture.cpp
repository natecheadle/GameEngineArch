#include "Texture.h"

namespace Ignosi::Modules::Render
{
    Texture::Texture(const std::string& name, enum TextureUnit unit)
        : ECS::Resource(name)
        , m_Unit(unit)
    {
    }

} // namespace Ignosi::Modules::Render
