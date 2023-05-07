#include "ShaderProgram.h"

#include <fmt/format.h>
#include <glad/glad.h>

#include <array>
#include <sstream>
#include <stdexcept>

namespace nate::Modules::Render
{
    ShaderProgram::ShaderProgram(
        const Shader* pFragmentShader,
        const Shader* pGeometryShader,
        const Shader* pVertexShader)
        : m_ID(glCreateProgram())
    {

        std::stringstream error;
        int               success{0};

        if (pFragmentShader)
        {
            glAttachShader(m_ID, pFragmentShader->ID());
        }

        if (pVertexShader)
        {
            glAttachShader(m_ID, pVertexShader->ID());
        }

        if (pGeometryShader)
        {
            glAttachShader(m_ID, pGeometryShader->ID());
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

    void ShaderProgram::Use() const
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

    void ShaderProgram::SetShaderVar(const std::string& name, const SquareMatrix<4, float>& value) const
    {
        auto loc = glGetUniformLocation(m_ID, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, value.raw_data());
    }
} // namespace nate::Modules::Render
