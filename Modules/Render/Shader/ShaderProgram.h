#pragma once

#include "3D/Light.h"
#include "3D/Light_Attenuation.h"
#include "3D/Light_Directional.h"
#include "3D/Light_Point.h"
#include "3D/Light_Spotlight.h"
#include "3D/Material.h"

#include <LinearAlgebra/SquareMatrix4x4.hpp>

#include <memory>

namespace Ignosi::Modules::Render
{
    class ShaderProgram
    {
      protected:
        ShaderProgram() = default;

      public:
        virtual ~ShaderProgram() = default;

        virtual void Use() const = 0;

        virtual void SetShaderVar(std::string_view name, bool value) const                          = 0;
        virtual void SetShaderVar(std::string_view name, int value) const                           = 0;
        virtual void SetShaderVar(std::string_view name, float value) const                         = 0;
        virtual void SetShaderVar(std::string_view name, const SquareMatrix<4, float>& value) const = 0;
        virtual void SetShaderVar(std::string_view name, const SquareMatrix<3, float>& value) const = 0;
        virtual void SetShaderVar(std::string_view name, const Vector<3, float>& value) const       = 0;
        virtual void SetShaderVar(std::string_view name, const Vector<4, float>& value) const       = 0;
        virtual void SetShaderVar(std::string_view name, const Material& value) const               = 0;
        virtual void SetShaderVar(std::string_view name, const Light_Directional& value) const      = 0;
        virtual void SetShaderVar(std::string_view name, const Light_Point& value) const            = 0;
        virtual void SetShaderVar(std::string_view name, const Light_Spotlight& value) const        = 0;
        virtual void SetShaderVar(std::string_view name, const Light_Attenuation& value) const      = 0;
        virtual void SetShaderVar(std::string_view name, const Light& value) const                  = 0;
    };
} // namespace Ignosi::Modules::Render