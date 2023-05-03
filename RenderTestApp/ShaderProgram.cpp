#include "ShaderProgram.h"

#include <fmt/format.h>
#include <glad/glad.h>

#include <sstream>
#include <stdexcept>

namespace nate::Modules::Render
{
    ShaderProgram::ShaderProgram(
        std::shared_ptr<Shader> pFragmentShader,
        std::shared_ptr<Shader> pGeometryShader,
        std::shared_ptr<Shader> pVertexShader)
        : m_ID(glCreateProgram())
        , m_pFragmentShader(std::move(pFragmentShader))
        , m_pGeometryShader(std::move(pGeometryShader))
        , m_pVertexShader(std::move(pVertexShader))
    {
        std::stringstream error;
        int               success{0};

        if (m_pFragmentShader)
        {
            glAttachShader(m_ID, m_pFragmentShader->ID());
        }

        if (m_pVertexShader)
        {
            glAttachShader(m_ID, m_pVertexShader->ID());
        }

        if (m_pGeometryShader)
        {
            glAttachShader(m_ID, m_pGeometryShader->ID());
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

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(m_ID);
    }

    void ShaderProgram::Use()
    {
        glUseProgram(m_ID);
    }

    void ShaderProgram::SetShaderVar(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), static_cast<int>(value));
    }

    void ShaderProgram::SetShaderVar(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void ShaderProgram::SetShaderVar(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
    }
} // namespace nate::Modules::Render
