#pragma once

#include "OpenGL_Shader.h"

namespace nate::Modules::Render
{
    class Fragment_OpenGL_Shader : public OpenGL_Shader
    {
      public:
        Fragment_OpenGL_Shader() = default;
        ShaderType Type() const override { return ShaderType::Fragment; };

      protected:
        unsigned int CreateGLShader() override;
    };
} // namespace nate::Modules::Render
