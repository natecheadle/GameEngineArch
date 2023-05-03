#pragma once
#include "Shader.h"

namespace nate::Modules::Render
{
    class ComputeShader : public Shader
    {
      public:
        ComputeShader() = default;
        ShaderType Type() const override { return ShaderType::Compute; };

      protected:
        unsigned int CreateGLShader() override;
    };
} // namespace nate::Modules::Render
