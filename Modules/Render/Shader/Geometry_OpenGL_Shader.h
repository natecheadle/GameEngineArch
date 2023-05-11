#pragma once

#include "OpenGL_Shader.h"

namespace nate::Modules::Render
{
    class Geometry_OpenGL_Shader : public OpenGL_Shader
    {
      public:
        Geometry_OpenGL_Shader() = default;
        ShaderType Type() const override { return ShaderType::Geometry; };

      protected:
        unsigned int CreateGLShader() override;
    };
} // namespace nate::Modules::Render
