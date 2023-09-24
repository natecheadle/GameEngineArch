#include "ImageFile.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <GLFW/glfw3.h>

#include <cassert>
#include <filesystem>
#include <stdexcept>

namespace Ignosi::Modules::Render
{
    ImageFile::ImageFile(const std::filesystem::path& path)
        : ECS::Resource(path.string())
        , m_Width(0)
        , m_Height(0)
        , m_Channels(0)
        , m_Format(GL_RED)
        , m_pData(nullptr)
        , m_Path(path)
    {
        if (!std::filesystem::is_regular_file(path))
            throw std::invalid_argument("Invalid file path. " + path.string());

        m_pData = stbi_load(path.string().c_str(), &m_Width, &m_Height, &m_Channels, 0);

        if (m_Channels == 1)
            m_Format = GL_RED;
        else if (m_Channels == 3)
            m_Format = GL_RGB;
        else if (m_Channels == 4)
            m_Format = GL_RGBA;
        else
            assert(false);

        assert(m_pData);
    }

    ImageFile::~ImageFile()
    {
        stbi_image_free(m_pData);
    }

} // namespace Ignosi::Modules::Render
