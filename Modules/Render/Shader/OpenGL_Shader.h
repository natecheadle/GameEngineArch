#pragma once

#include "Shader.h"

#include <vector>

namespace nate::Modules::Render
{
    class OpenGL_Shader : public Shader
    {
      public:
      private:
        unsigned int m_ID{0};

      public:
        unsigned int ID() const { return m_ID; }

        ~OpenGL_Shader() override;

        static std::unique_ptr<Shader> Create(
            const std::filesystem::path&              shaderLoc,
            const std::vector<std::filesystem::path>& inc_paths = std::vector<std::filesystem::path>());
        static std::unique_ptr<Shader> Create(
            const std::filesystem::path&              shaderLoc,
            ShaderType                                type,
            const std::vector<std::filesystem::path>& inc_paths = std::vector<std::filesystem::path>());

      protected:
        OpenGL_Shader() = default;

        virtual unsigned int CreateGLShader() = 0;

      private:
        void ID(unsigned int val) { m_ID = val; }
        void Compile(const std::vector<std::filesystem::path>& inc_paths);
    };
} // namespace nate::Modules::Render