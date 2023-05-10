#pragma once

#include "SquareMatrix4x4.hpp"

#include <memory>

namespace nate::Modules::Render
{
    class ShaderProgram
    {
      protected:
        ShaderProgram() = default;

      public:
        virtual ~ShaderProgram() = default;

        virtual void Use() const = 0;

        virtual void SetShaderVar(const std::string& name, bool value) const                          = 0;
        virtual void SetShaderVar(const std::string& name, int value) const                           = 0;
        virtual void SetShaderVar(const std::string& name, float value) const                         = 0;
        virtual void SetShaderVar(const std::string& name, const SquareMatrix<4, float>& value) const = 0;
        virtual void SetShaderVar(const std::string& name, const Vector<3, float>& value) const       = 0;
        virtual void SetShaderVar(const std::string& name, const Vector<4, float>& value) const       = 0;
    };
} // namespace nate::Modules::Render