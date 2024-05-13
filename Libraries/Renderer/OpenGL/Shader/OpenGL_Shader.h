#pragma once

#include "Shader/Shader.h"

#include <vector>

namespace Ignosi::Libraries::Renderer
{
    class OpenGL_Shader : public Shader
    {
      public:
      private:
        unsigned int m_ID{0};

      public:
        unsigned int ID() const { return m_ID; }

        ~OpenGL_Shader() override;

        static std::unique_ptr<IShader> Create(
            const std::filesystem::path&              shaderLoc,
            const std::vector<std::filesystem::path>& inc_paths = std::vector<std::filesystem::path>());
        static std::unique_ptr<IShader> Create(
            const std::filesystem::path&              shaderLoc,
            ShaderType                                type,
            const std::vector<std::filesystem::path>& inc_paths = std::vector<std::filesystem::path>());

      protected:
        OpenGL_Shader(const std::filesystem::path& shaderLoc);

        virtual unsigned int CreateGLShader() = 0;

      private:
        void ID(unsigned int val) { m_ID = val; }
        void Compile(const std::vector<std::filesystem::path>& inc_paths);
    };
} // namespace Ignosi::Libraries::Renderer
