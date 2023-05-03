#pragma once

#pragma once

#include "Shader.h"

namespace nate::Modules::Render
{
    class VertexShader : public Shader
    {
      public:
        VertexShader() = default;

        ShaderType Type() const override { return ShaderType::Vertex; };

      protected:
        unsigned int CreateGLShader() override;
    };
} // namespace nate::Modules::Render