#pragma once

#include "Shader.h"
#include "ShaderProgram.h"

#include <LinearAlgebra/SquareMatrix4x4.hpp>

#include <memory>

namespace nate::Modules::Render
{
    class OpenGL_ShaderProgram : public ShaderProgram
    {
        unsigned int m_ID;

      public:
        OpenGL_ShaderProgram(const Shader* pFragmentShader, const Shader* pGeometryShader, const Shader* pVertexShader);

        ~OpenGL_ShaderProgram() override;

        void         Use() const override;
        unsigned int ID() const { return m_ID; }

        void SetShaderVar(std::string_view name, bool value) const final;
        void SetShaderVar(std::string_view name, int value) const final;
        void SetShaderVar(std::string_view name, float value) const final;
        void SetShaderVar(std::string_view name, const SquareMatrix<4, float>& value) const final;
        void SetShaderVar(std::string_view name, const SquareMatrix<3, float>& value) const final;
        void SetShaderVar(std::string_view name, const Vector<3, float>& value) const final;
        void SetShaderVar(std::string_view name, const Vector<4, float>& value) const final;
        void SetShaderVar(std::string_view name, const Material& value) const final;
        void SetShaderVar(std::string_view name, const Light_Directional& value) const final;
        void SetShaderVar(std::string_view name, const Light_Point& value) const final;
        void SetShaderVar(std::string_view name, const Light_Spotlight& value) const final;
        void SetShaderVar(std::string_view name, const Light_Attenuation& value) const final;
        void SetShaderVar(std::string_view name, const Light& value) const final;
    };
} // namespace nate::Modules::Render