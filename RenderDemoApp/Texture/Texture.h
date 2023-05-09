#pragma once
#include "ImageFile.h"

#include <filesystem>
#include <string>

namespace nate::Modules::Render
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

    class Texture
    {
      public:
      private:
        const unsigned int m_ID;
        const TextureUnit  m_Unit;
        const unsigned int m_UnitID;

      public:
        Texture(const std::filesystem::path& path, TextureUnit unit);
        Texture(const ImageFile& image, TextureUnit unit);

        ~Texture();

        unsigned int GetID() const { return m_ID; }

        void Activate() const;
        void Bind() const;

      protected:
        static unsigned int CreateTexture();
        void                InitializeFromImage(const ImageFile& image) const;

      private:
        static unsigned int TranslateTextureUnit(TextureUnit unit);
    };
} // namespace nate::Modules::Render
