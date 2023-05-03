#include "Shader.h"

#include "ComputeShader.h"
#include "FragmentShader.h"
#include "GeometryShader.h"
#include "VertexShader.h"

#include <fmt/format.h>
#include <glad/glad.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace nate::Modules::Render
{
    Shader::~Shader()
    {
        glDeleteShader(m_ID);
    }

    std::unique_ptr<Shader> Shader::Create(const std::filesystem::path& shaderLoc)
    {
        std::string extension = shaderLoc.extension().string();
        if (extension == ".vert")
        {
            return Create(shaderLoc, ShaderType::Vertex);
        }
        if (extension == ".frag")
        {
            return Create(shaderLoc, ShaderType::Fragment);
        }
        if (extension == ".geom")
        {
            return Create(shaderLoc, ShaderType::Geometry);
        }
        if (extension == ".comp")
        {
            return Create(shaderLoc, ShaderType::Compute);
        }
        throw std::invalid_argument("Unkown shader extension " + extension);
    }

    std::unique_ptr<Shader> Shader::Create(const std::filesystem::path& shaderLoc, ShaderType type)
    {
        if (!std::filesystem::is_regular_file(shaderLoc))
            throw std::invalid_argument(fmt::format("Shader file [{}] does not exist.", shaderLoc.string()));

        std::unique_ptr<Shader> pNewObj;

        switch (type)
        {
        case ShaderType::Fragment: pNewObj = std::make_unique<FragmentShader>(); break;
        case ShaderType::Vertex: pNewObj = std::make_unique<VertexShader>(); break;
        case ShaderType::Compute: pNewObj = std::make_unique<ComputeShader>(); break;
        case ShaderType::Geometry: pNewObj = std::make_unique<GeometryShader>(); break;
        default: throw std::invalid_argument(fmt::format("Unsupported Shader Type {}.", int(type)));
        }

        pNewObj->LoadShaderCode(shaderLoc);
        pNewObj->ID(pNewObj->CreateGLShader());
        pNewObj->Compile();

        return pNewObj;
    }

    void Shader::Compile()
    {
        const char* pStr = m_ShaderCode.c_str();

        glShaderSource(m_ID, 1, &pStr, NULL);
        glCompileShader(m_ID);

        int  success{0};
        char infoLog[512];
        glGetShaderiv(m_ID, GL_COMPILE_STATUS, &success);

        if (success == 0)
        {
            throw std::runtime_error(fmt::format("Compilation failed, {}.", infoLog));
        }
    }

    void Shader::LoadShaderCode(const std::filesystem::path& shaderLoc)
    {
        std::ifstream shaderFile(shaderLoc);
        if (!shaderFile.is_open())
            throw std::invalid_argument(fmt::format("Shader file [{}] does not exist.", shaderLoc.string()));

        std::stringstream sstream;
        sstream << shaderFile.rdbuf();
        m_ShaderCode = sstream.str();
    }
} // namespace nate::Modules::Render
