#include "OpenGL_ShaderProgram.h"

#include "OpenGL_Shader.h"

#include <DebugCast.hpp>
#include <fmt/format.h>
#include <glad/glad.h>

#include <array>
#include <sstream>
#include <stdexcept>

namespace nate::Modules::Render
{
    OpenGL_ShaderProgram::OpenGL_ShaderProgram(
        const Shader* pFragmentShader,
        const Shader* pGeometryShader,
        const Shader* pVertexShader)
        : m_ID(glCreateProgram())
    {
        std::stringstream error;
        int               success{0};

        if (pFragmentShader)
        {
            const auto* pGL_FragShader = DebugCast<const OpenGL_Shader*>(pFragmentShader);
            glAttachShader(m_ID, pGL_FragShader->ID());
        }

        if (pVertexShader)
        {
            const auto* pGL_VertShader = DebugCast<const OpenGL_Shader*>(pVertexShader);
            glAttachShader(m_ID, pGL_VertShader->ID());
        }

        if (pGeometryShader)
        {
            const auto* pGL_GeoShader = DebugCast<const OpenGL_Shader*>(pGeometryShader);
            glAttachShader(m_ID, pGL_GeoShader->ID());
        }

        glLinkProgram(m_ID);

        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (success == 0)
        {
            char infoLog[512];
            glGetProgramInfoLog(m_ID, sizeof(infoLog), NULL, infoLog);
            throw std::runtime_error(fmt::format("Linking Shader Program Failed: {}", infoLog));
        }
    }

    OpenGL_ShaderProgram::~OpenGL_ShaderProgram()
    {
        glDeleteProgram(m_ID);
    }

    void OpenGL_ShaderProgram::Use() const
    {
        glUseProgram(m_ID);
    }

    void OpenGL_ShaderProgram::SetShaderVar(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), static_cast<int>(value));
    }

    void OpenGL_ShaderProgram::SetShaderVar(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void OpenGL_ShaderProgram::SetShaderVar(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void OpenGL_ShaderProgram::SetShaderVar(const std::string& name, const SquareMatrix<4, float>& value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, value.raw_data());
    }

    void OpenGL_ShaderProgram::SetShaderVar(const std::string& name, const SquareMatrix<3, float>& value) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, value.raw_data());
    }

    void OpenGL_ShaderProgram::SetShaderVar(const std::string& name, const Vector<3, float>& value) const
    {
        glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, value.data());
    }

    void OpenGL_ShaderProgram::SetShaderVar(const std::string& name, const Vector<4, float>& value) const
    {
        glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, value.data());
    }

    void OpenGL_ShaderProgram::SetShaderVar(const std::string& name, const Material& value) const
    {
        SetShaderVar(name + ".Diffuse", static_cast<int>(value.Diffuse->TextureUnit()));
        SetShaderVar(name + ".Specular", static_cast<int>(value.Specular->TextureUnit()));
        SetShaderVar(name + ".Shininess", value.Shininess);
    }
    void OpenGL_ShaderProgram::SetShaderVar(const std::string& name, const Light_Directional& value) const
    {
        SetShaderVar(name + ".Light", value.Light);
        SetShaderVar(name + ".Direction", value.Direction);
    }

    void OpenGL_ShaderProgram::SetShaderVar(const std::string& name, const Light_Point& value) const
    {
        SetShaderVar(name + ".Light", value.Light);
        SetShaderVar(name + ".Position", value.Position);
        SetShaderVar(name + ".Attenuation", value.Attenuation);
    }

    void OpenGL_ShaderProgram::SetShaderVar(const std::string& name, const Light_Spotlight& value) const
    {
        SetShaderVar(name + ".Light", value.Light);

        SetShaderVar(name + ".Position", value.Position);
        SetShaderVar(name + ".Direction", value.Direction);
        SetShaderVar(name + ".CutOff", cos(value.Cutoff));
        SetShaderVar(name + ".OuterCutOff", cos(value.OuterCutoff));

        SetShaderVar(name + ".Attenuation", value.Attenuation);
    }

    void OpenGL_ShaderProgram::SetShaderVar(const std::string& name, const Light_Attenuation& value) const
    {
        SetShaderVar(name + ".Constant", value.Constant);
        SetShaderVar(name + ".Linear", value.Linear);
        SetShaderVar(name + ".Quadratic", value.Quadratic);
    }

    void OpenGL_ShaderProgram::SetShaderVar(const std::string& name, const Light& value) const
    {
        SetShaderVar(name + ".Ambient", value.Ambient.Data());
        SetShaderVar(name + ".Diffuse", value.Diffuse.Data());
        SetShaderVar(name + ".Specular", value.Specular.Data());
    }

} // namespace nate::Modules::Render
