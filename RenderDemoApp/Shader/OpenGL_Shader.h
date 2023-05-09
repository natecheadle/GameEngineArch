#pragma once

#include "Shader.h"

namespace nate::Modules::Render
{
    class OpenGL_Shader : public Shader
    {
      public:
      private:
        std::string  m_ShaderCode;
        unsigned int m_ID{0};

      public:
        const std::string& ShaderCode() const final { return m_ShaderCode; }
        unsigned int       ID() const { return m_ID; }

        ~OpenGL_Shader() override;

        static std::unique_ptr<Shader> Create(const std::filesystem::path& shaderLoc);
        static std::unique_ptr<Shader> Create(const std::filesystem::path& shaderLoc, ShaderType type);

      protected:
        OpenGL_Shader() = default;

        virtual unsigned int CreateGLShader() = 0;

      private:
        void LoadShaderCode(const std::filesystem::path& shaderLoc);
        void ID(unsigned int val) { m_ID = val; }
        void Compile();
    };
} // namespace nate::Modules::Render