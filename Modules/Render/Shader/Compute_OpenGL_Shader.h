#pragma once

#include "OpenGL_Shader.h"

namespace nate::Modules::Render
{
    class Compute_OpenGL_Shader : public OpenGL_Shader
    {
      public:
        Compute_OpenGL_Shader() = default;
        ShaderType Type() const override { return ShaderType::Compute; };

      protected:
        unsigned int CreateGLShader() override;
    };
} // namespace nate::Modules::Render
