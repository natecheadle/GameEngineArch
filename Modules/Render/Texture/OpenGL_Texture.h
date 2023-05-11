#pragma once

#include "Texture.h"

namespace nate::Modules::Render
{
    class OpenGL_Texture : public Texture
    {
      public:
      private:
        const unsigned int m_ID;
        const unsigned int m_UnitID;

      public:
        OpenGL_Texture(const std::filesystem::path& path, TextureUnit unit);
        OpenGL_Texture(const ImageFile& image, TextureUnit unit);

        ~OpenGL_Texture() override;

        unsigned int GetID() const { return m_ID; }

        void Activate() const override;
        void Bind() const override;

      protected:
        static unsigned int CreateTexture();
        void                InitializeFromImage(const ImageFile& image) const;

      private:
        static unsigned int TranslateTextureUnit(TextureUnit unit);
    };
} // namespace nate::Modules::Render
