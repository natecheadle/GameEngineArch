#pragma once

#pragma once

#include "OpenGL_Shader.h"

namespace nate::Modules::Render
{
    class Vertex_OpenGL_Shader : public OpenGL_Shader
    {
      public:
        Vertex_OpenGL_Shader() = default;

        ShaderType Type() const override { return ShaderType::Vertex; };

      protected:
        unsigned int CreateGLShader() override;
    };
} // namespace nate::Modules::Render