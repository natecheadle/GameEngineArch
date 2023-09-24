#pragma once

#pragma once

#include "OpenGL_Shader.h"

namespace Ignosi::Modules::Render
{
    class Vertex_OpenGL_Shader : public OpenGL_Shader
    {
      public:
        Vertex_OpenGL_Shader(const std::filesystem::path& shaderLoc);

        ShaderType Type() const override { return ShaderType::Vertex; };

      protected:
        unsigned int CreateGLShader() override;
    };
} // namespace Ignosi::Modules::Render
