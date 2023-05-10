#pragma once

#include <filesystem>
#include <string>

namespace nate::Modules::Render
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
        std::string m_ShaderCode;

      public:
        const std::string& ShaderCode() const { return m_ShaderCode; }
        virtual ShaderType Type() const = 0;

        virtual ~Shader() = default;

      protected:
        Shader() = default;
        void LoadShaderCode(const std::filesystem::path& shaderLoc);
    };
} // namespace nate::Modules::Render