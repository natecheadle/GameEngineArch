#include "Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <GLFW/glfw3.h>

#include <cassert>
#include <filesystem>

namespace nate::Modules::Render
{
    Texture::Texture(const std::filesystem::path& path, TextureUnit unit)
        : m_ID(CreateTexture())
        , m_Unit(unit)
        , m_UnitID(TranslateTextureUnit(unit))
    {
        ImageFile image(path);
        InitializeFromImage(image);
    }

    Texture::Texture(const ImageFile& image, TextureUnit unit)
        : m_ID(CreateTexture())
        , m_Unit(unit)
        , m_UnitID(TranslateTextureUnit(unit))
    {
        InitializeFromImage(image);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_ID);
    }

    unsigned int Texture::CreateTexture()
    {
        unsigned int id;
        glGenTextures(1, &id);
        return id;
    }

    void Texture::InitializeFromImage(const ImageFile& image) const
    {
        glBindTexture(GL_TEXTURE_2D, m_ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        assert(image.GetData());

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            image.GetWidth(),
            image.GeHeight(),
            0,
            image.GetFormat(),
            GL_UNSIGNED_BYTE,
            image.GetData());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture::Activate() const
    {
        glActiveTexture(m_UnitID);
    }
    void Texture::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }

    unsigned int Texture::TranslateTextureUnit(TextureUnit unit)
    {
        switch (unit)
        {
        case TextureUnit::Texture0: return GL_TEXTURE0;
        case TextureUnit::Texture1: return GL_TEXTURE1;
        case TextureUnit::Texture2: return GL_TEXTURE2;
        case TextureUnit::Texture3: return GL_TEXTURE3;
        case TextureUnit::Texture4: return GL_TEXTURE4;
        case TextureUnit::Texture5: return GL_TEXTURE5;
        case TextureUnit::Texture6: return GL_TEXTURE6;
        case TextureUnit::Texture7: return GL_TEXTURE7;
        case TextureUnit::Texture8: return GL_TEXTURE8;
        case TextureUnit::Texture9: return GL_TEXTURE9;
        case TextureUnit::Texture10: return GL_TEXTURE10;
        case TextureUnit::Texture11: return GL_TEXTURE11;
        case TextureUnit::Texture12: return GL_TEXTURE12;
        case TextureUnit::Texture13: return GL_TEXTURE13;
        case TextureUnit::Texture14: return GL_TEXTURE14;
        case TextureUnit::Texture15: return GL_TEXTURE15;
        case TextureUnit::Texture16: return GL_TEXTURE16;
        default: assert(false);
        }

        return GL_TEXTURE0;
    }

} // namespace nate::Modules::Render