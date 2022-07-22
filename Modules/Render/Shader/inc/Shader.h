#pragma once

#include <bgfx/bgfx.h>

#include <filesystem>
#include <fstream>
#include <string_view>

namespace nate::Modules::Render {
    class Shader {
      public:
        Shader(std::string_view name, std::filesystem::path shadersPath);
        ~Shader();

        bgfx::ShaderHandle           Handle() const { return m_Handle; }
        const std::filesystem::path& Path() const { return m_Path; }

      private:
        std::filesystem::path m_Path;
        bgfx::ShaderHandle    m_Handle;

        static bgfx::ShaderHandle    CreateHandle(const std::filesystem::path& path);
        static std::filesystem::path GetFullPath(std::string_view name, std::filesystem::path shadersPath);
    };
} // namespace nate::Modules::Render