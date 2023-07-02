#include "OpenGL_ShaderProgram.h"

#include "OpenGL_Shader.h"
#include "Texture/Texture.h"

#include <DebugCast.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#include <glad/glad.h>

#include <array>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace Ignosi::Modules::Render
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

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_ID, name.data()), static_cast<int>(value));
    }

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_ID, name.data()), value);
    }

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, float value) const
    {
        glUniform1f(glGetUniformLocation(m_ID, name.data()), value);
    }

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, const SquareMatrix<4, float>& value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.data()), 1, GL_FALSE, value.raw_data());
    }

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, const SquareMatrix<3, float>& value) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_ID, name.data()), 1, GL_FALSE, value.raw_data());
    }

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, const Vector<3, float>& value) const
    {
        glUniform3fv(glGetUniformLocation(m_ID, name.data()), 1, value.data());
    }

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, const Vector<4, float>& value) const
    {
        glUniform4fv(glGetUniformLocation(m_ID, name.data()), 1, value.data());
    }

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, const Material& value) const
    {
        auto populateShader = [&](const std::shared_ptr<Texture>& texs, std::string_view field) -> void {
            SetShaderVar(fmt::format("{}.{}", name, field), static_cast<int>(texs->TextureUnit()));
        };

        if (value.Diffuse)
        {
            populateShader(value.Diffuse, "Diffuse");
        }
        if (value.Specular)
        {
            populateShader(value.Specular, "Specular");
        }
        if (value.Height)
        {
            populateShader(value.Height, "Height");
        }
        if (value.Normal)
        {
            populateShader(value.Normal, "Normal");
        }

        SetShaderVar(fmt::format("{}.{}", name, "Shininess"), value.Shininess);
    }
    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, const Light_Directional& value) const
    {
        SetShaderVar(fmt::format("{}.{}", name, "Light"), value.Light);
        SetShaderVar(fmt::format("{}.{}", name, "Direction"), value.Direction);
    }

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, const Light_Point& value) const
    {
        SetShaderVar(fmt::format("{}.{}", name, "Light"), value.Light);
        SetShaderVar(fmt::format("{}.{}", name, "Position"), value.Position);
        SetShaderVar(fmt::format("{}.{}", name, "Attenuation"), value.Attenuation);
    }

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, const Light_Spotlight& value) const
    {
        SetShaderVar(fmt::format("{}.{}", name, "Light"), value.Light);

        SetShaderVar(fmt::format("{}.{}", name, "Position"), value.Position);
        SetShaderVar(fmt::format("{}.{}", name, "Direction"), value.Direction);
        SetShaderVar(fmt::format("{}.{}", name, "CutOff"), cos(value.Cutoff));
        SetShaderVar(fmt::format("{}.{}", name, "OuterCutOff"), cos(value.OuterCutoff));

        SetShaderVar(fmt::format("{}.{}", name, "Attenuation"), value.Attenuation);
    }

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, const Light_Attenuation& value) const
    {
        SetShaderVar(fmt::format("{}.{}", name, "Constant"), value.Constant);
        SetShaderVar(fmt::format("{}.{}", name, "Linear"), value.Linear);
        SetShaderVar(fmt::format("{}.{}", name, "Quadratic"), value.Quadratic);
    }

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, const Light& value) const
    {
        SetShaderVar(fmt::format("{}.{}", name, "Ambient"), value.Ambient.Data());
        SetShaderVar(fmt::format("{}.{}", name, "Diffuse"), value.Diffuse.Data());
        SetShaderVar(fmt::format("{}.{}", name, "Specular"), value.Specular.Data());
    }

} // namespace Ignosi::Modules::Render
