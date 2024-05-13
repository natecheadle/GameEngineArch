#pragma once

#include "OpenGL_Shader.h"

namespace Ignosi::Libraries::Renderer
{
    class Geometry_OpenGL_Shader : public OpenGL_Shader
    {
      public:
        Geometry_OpenGL_Shader(const std::filesystem::path& shaderLoc);

        ShaderType Type() const override { return ShaderType::Geometry; };

      protected:
        unsigned int CreateGLShader() override;
    };
} // namespace Ignosi::Modules::Render
