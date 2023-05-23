#pragma once
#include "ImageFile.h"

#include <filesystem>
#include <string>

namespace nate::Modules::Render
{
    enum class TextureUnit
    {
        Texture0 = 0,
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

    class Texture
    {
      private:
        const TextureUnit m_Unit;

      protected:
        Texture(TextureUnit unit);

      public:
        virtual ~Texture() = default;

        TextureUnit  TextureUnit() const { return m_Unit; }
        virtual void Activate() const = 0;
        virtual void Bind() const     = 0;
        virtual void Unbind() const   = 0;
    };
} // namespace nate::Modules::Render
