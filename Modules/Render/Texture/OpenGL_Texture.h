#pragma once

#include "Texture.h"

namespace Ignosi::Modules::Render
{
    class OpenGL_Texture : public Texture
    {
      public:
      private:
        const unsigned int m_ID;
        const unsigned int m_UnitID;

      public:
        OpenGL_Texture(const std::string& textureName, const std::filesystem::path& imagePath, enum TextureUnit unit);
        OpenGL_Texture(const std::string& textureName, const ImageFile& image, enum TextureUnit unit);

        ~OpenGL_Texture() override;

        unsigned int GetID() const { return m_ID; }

        void Activate() const override;
        void Bind() const override;

      protected:
        static unsigned int CreateTexture();
        void                InitializeFromImage(const ImageFile& image) const;

      private:
        static unsigned int TranslateTextureUnit(enum TextureUnit unit);
    };
} // namespace Ignosi::Modules::Render
