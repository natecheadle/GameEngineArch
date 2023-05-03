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
      public:
      private:
        std::string  m_ShaderCode;
        unsigned int m_ID{0};

      public:
        const std::string& ShaderCode() const { return m_ShaderCode; }
        virtual ShaderType Type() const = 0;
        unsigned int       ID() const { return m_ID; }

        virtual ~Shader();

        static std::unique_ptr<Shader> Create(const std::filesystem::path& shaderLoc);
        static std::unique_ptr<Shader> Create(const std::filesystem::path& shaderLoc, ShaderType type);

      protected:
        Shader() = default;

        virtual unsigned int CreateGLShader() = 0;

      private:
        void LoadShaderCode(const std::filesystem::path& shaderLoc);
        void ID(unsigned int val) { m_ID = val; }
        void Compile();
    };
} // namespace nate::Modules::Render