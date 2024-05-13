#pragma once

#include "OpenGL_Shader.h"

namespace Ignosi::Libraries::Renderer
{
    class Compute_OpenGL_Shader : public OpenGL_Shader
    {
      public:
        Compute_OpenGL_Shader(const std::filesystem::path& shaderLoc);

        ShaderType Type() const override { return ShaderType::Compute; };

      protected:
        unsigned int CreateGLShader() override;
    };
} // namespace Ignosi::Modules::Render
