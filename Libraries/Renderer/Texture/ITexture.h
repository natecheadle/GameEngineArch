#pragma once

namespace Ignosi::Libraries::Renderer
{
    enum class TextureUnit : unsigned int
    {
        Texture0,
        Texture1,
        Texture2,
        Texture3,
        Texture4,
        Texture5,
        Texture6,
        Texture7,
        Texture8,
        Texture9,
        Texture10,
        Texture11,
        Texture12,
        Texture13,
        Texture14,
        Texture15,
        Texture16,
    };

    class ITexture
    {
      public:
        ITexture()          = default;
        virtual ~ITexture() = default;

        virtual enum TextureUnit TextureUnit() const = 0;
        virtual void             Activate() const    = 0;
        virtual void             Bind() const        = 0;
    };
} // namespace Ignosi::Libraries::Renderer
