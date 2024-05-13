#pragma once

#include "Shader/IShader.h"
#include "Shader/IShaderProgram.h"

#include <LinearAlgebra/SquareMatrix4x4.hpp>

namespace Ignosi::Libraries::Renderer
{
    class OpenGL_ShaderProgram : public IShaderProgram
    {
        unsigned int m_ID;

        std::shared_ptr<const IShader> m_pFragmentShader;
        std::shared_ptr<const IShader> m_pGeometryShader;
        std::shared_ptr<const IShader> m_pVertexShader;

      public:
        OpenGL_ShaderProgram(
            std::shared_ptr<const IShader> pFragmentShader,
            std::shared_ptr<const IShader> pGeometryShader,
            std::shared_ptr<const IShader> pVertexShader);

        ~OpenGL_ShaderProgram() override;

        void         Use() const override;
        unsigned int ID() const { return m_ID; }

        void SetShaderVar(std::string_view name, bool value) const final;
        void SetShaderVar(std::string_view name, int value) const final;
        void SetShaderVar(std::string_view name, float value) const final;
        void SetShaderVar(std::string_view name, const Math::SquareMatrix<4, float>& value) const final;
        void SetShaderVar(std::string_view name, const Math::SquareMatrix<3, float>& value) const final;
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
