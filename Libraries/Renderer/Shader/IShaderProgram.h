#pragma once

#include "Lighting/Light.h"
#include "Lighting/Light_Attenuation.h"
#include "Lighting/Light_Directional.h"
#include "Lighting/Light_Point.h"
#include "Lighting/Light_Spotlight.h"
#include "Texture/IMaterial.h"

#include <LinearAlgebra/SquareMatrix.hpp>

namespace Ignosi::Libraries::Renderer
{
    class IShaderProgram
    {
      public:
        IShaderProgram()          = default;
        virtual ~IShaderProgram() = default;

        virtual void Use() const = 0;

        virtual void SetShaderVar(std::string_view name, bool value) const                                = 0;
        virtual void SetShaderVar(std::string_view name, int value) const                                 = 0;
        virtual void SetShaderVar(std::string_view name, float value) const                               = 0;
        virtual void SetShaderVar(std::string_view name, const Math::SquareMatrix4x4<float>& value) const = 0;
        virtual void SetShaderVar(std::string_view name, const Math::SquareMatrix3x3<float>& value) const = 0;
        virtual void SetShaderVar(std::string_view name, const Math::Vector<3, float>& value) const       = 0;
        virtual void SetShaderVar(std::string_view name, const Math::Vector<4, float>& value) const       = 0;
        virtual void SetShaderVar(std::string_view name, const IMaterial& value) const                    = 0;
        virtual void SetShaderVar(std::string_view name, const Light_Directional& value) const            = 0;
        virtual void SetShaderVar(std::string_view name, const Light_Point& value) const                  = 0;
        virtual void SetShaderVar(std::string_view name, const Light_Spotlight& value) const              = 0;
        virtual void SetShaderVar(std::string_view name, const Light_Attenuation& value) const            = 0;
        virtual void SetShaderVar(std::string_view name, const Light& value) const                        = 0;
    };
} // namespace Ignosi::Libraries::Renderer
