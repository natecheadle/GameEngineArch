#pragma once

#include "Shader/IShader.h"
#include "Shader/IShaderProgram.h"

#include <LinearAlgebra/SquareMatrix.hpp>

namespace Ignosi::Libraries::Renderer
{
    class OpenGL_ShaderProgram : public IShaderProgram
    {
        unsigned int m_ID;

        const IShader* m_pFragmentShader;
        const IShader* m_pGeometryShader;
        const IShader* m_pVertexShader;

      public:
        OpenGL_ShaderProgram(const IShader* pFragmentShader, const IShader* pGeometryShader, const IShader* pVertexShader);

        ~OpenGL_ShaderProgram() override;

        void         Use() const override;
        unsigned int ID() const { return m_ID; }

        void SetShaderVar(std::string_view name, bool value) const final;
        void SetShaderVar(std::string_view name, int value) const final;
        void SetShaderVar(std::string_view name, float value) const final;
        void SetShaderVar(std::string_view name, const Math::SquareMatrix4x4<float>& value) const final;
        void SetShaderVar(std::string_view name, const Math::SquareMatrix3x3<float>& value) const final;
        void SetShaderVar(std::string_view name, const Math::Vector<3, float>& value) const final;
        void SetShaderVar(std::string_view name, const Math::Vector<4, float>& value) const final;
        void SetShaderVar(std::string_view name, const IMaterial& value) const final;
        void SetShaderVar(std::string_view name, const Light_Directional& value) const final;
        void SetShaderVar(std::string_view name, const Light_Point& value) const final;
        void SetShaderVar(std::string_view name, const Light_Spotlight& value) const final;
        void SetShaderVar(std::string_view name, const Light_Attenuation& value) const final;
        void SetShaderVar(std::string_view name, const Light& value) const final;
    };
} // namespace Ignosi::Libraries::Renderer
