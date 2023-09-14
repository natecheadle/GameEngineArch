#pragma once

#include <filesystem>

namespace Ignosi::Modules::Render
{
    class ImageFile
    {

        int                   m_Width;
        int                   m_Height;
        int                   m_Channels;
        int                   m_Format;
        unsigned char*        m_pData;
        std::filesystem::path m_Path;

      public:
        ImageFile(const std::filesystem::path& path);
        ~ImageFile();

        const unsigned char*         Data() const { return m_pData; }
        int                          Width() const { return m_Width; }
        int                          Height() const { return m_Width; }
        int                          Channels() const { return m_Width; }
        int                          Format() const { return m_Format; }
        const std::filesystem::path& Path() const { return m_Path; }
        double                       AspectRatio() const { return double(m_Width) / double(m_Height); }
    };
} // namespace Ignosi::Modules::Render
