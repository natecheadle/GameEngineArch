#pragma once

#include <filesystem>
#include <string>

namespace Ignosi::Modules::Render
{
    enum class ShaderType
    {
        Vertex,
        Fragment,
        Geometry,
        Compute,

        LAST
    };

    class Shader
    {
      private:
        std::filesystem::path m_ShaderLoc;
        std::string           m_ShaderCode;

      public:
        const std::string&           ShaderCode() const { return m_ShaderCode; }
        const std::filesystem::path& ShaderLoc() const { return m_ShaderLoc; }
        virtual ShaderType           Type() const = 0;

        virtual ~Shader() = default;

      protected:
        Shader() = default;
        void LoadShaderCode(const std::filesystem::path& shaderLoc);
    };
} // namespace Ignosi::Modules::Render