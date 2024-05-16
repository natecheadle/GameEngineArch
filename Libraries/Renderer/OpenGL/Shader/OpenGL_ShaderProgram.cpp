#include "OpenGL_ShaderProgram.h"

#include "OpenGL_Shader.h"
#include "Texture/Texture.h"

#include <fmt/format.h>
#include <glad/glad.h>

#include <memory>
#include <stdexcept>

namespace Ignosi::Libraries::Renderer
{
    OpenGL_ShaderProgram::OpenGL_ShaderProgram(const IShader* pFragmentShader, const IShader* pGeometryShader, const IShader* pVertexShader)
        : m_ID(glCreateProgram())
        , m_pFragmentShader(pFragmentShader)
        , m_pGeometryShader(pGeometryShader)
        , m_pVertexShader(pVertexShader)

    {
        int success{0};

        if (m_pFragmentShader)
        {
            const auto* pGL_FragShader = dynamic_cast<const OpenGL_Shader*>(m_pFragmentShader);
            glAttachShader(m_ID, pGL_FragShader->ID());
        }

        if (m_pVertexShader)
        {
            const auto* pGL_VertShader = dynamic_cast<const OpenGL_Shader*>(m_pVertexShader);
            glAttachShader(m_ID, pGL_VertShader->ID());
        }

        if (m_pGeometryShader)
        {
            const auto* pGL_GeoShader = dynamic_cast<const OpenGL_Shader*>(m_pGeometryShader);
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

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, const Math::SquareMatrix<4, float>& value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.data()), 1, GL_FALSE, value.raw_data());
    }

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, const Math::SquareMatrix<3, float>& value) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_ID, name.data()), 1, GL_FALSE, value.raw_data());
    }

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, const Math::Vector<3, float>& value) const
    {
        glUniform3fv(glGetUniformLocation(m_ID, name.data()), 1, value.data());
    }

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, const Math::Vector<4, float>& value) const
    {
        glUniform4fv(glGetUniformLocation(m_ID, name.data()), 1, value.data());
    }

    void OpenGL_ShaderProgram::SetShaderVar(std::string_view name, const IMaterial& value) const
    {
        auto populateShader = [&](ITexture* texs, std::string_view field) -> void {
            if (texs)
            {
                SetShaderVar(fmt::format("{}.{}", name, field), static_cast<int>(texs->TextureUnit()));
            }
        };

        populateShader(value.Diffuse(), "Diffuse");
        populateShader(value.Specular(), "Specular");
        populateShader(value.Height(), "Height");
        populateShader(value.Normal(), "Normal");

        SetShaderVar(fmt::format("{}.{}", name, "Shininess"), value.Shininess());
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

} // namespace Ignosi::Libraries::Renderer
