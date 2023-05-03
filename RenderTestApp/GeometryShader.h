#pragma once
#include "Shader.h"

namespace nate::Modules::Render
{
    class GeometryShader : public Shader
    {
      public:
        GeometryShader() = default;
        ShaderType Type() const override { return ShaderType::Geometry; };

      protected:
        unsigned int CreateGLShader() override;
    };
} // namespace nate::Modules::Render
