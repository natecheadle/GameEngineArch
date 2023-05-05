#pragma once

#include "Shader.h"

namespace nate::Modules::Render
{
    class FragmentShader : public Shader
    {
      public:
        FragmentShader() = default;
        ShaderType Type() const override { return ShaderType::Fragment; };

      protected:
        unsigned int CreateGLShader() override;
    };
} // namespace nate::Modules::Render
