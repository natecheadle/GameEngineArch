#include "OpenGL_Shader.h"

#include "Compute_OpenGL_Shader.h"
#include "Fragment_OpenGL_Shader.h"
#include "Geometry_OpenGL_Shader.h"
#include "Preprocessor.h"
#include "Vertex_OpenGL_Shader.h"

#include <fmt/format.h>
#include <glad/glad.h>

#include <iostream>
#include <stdexcept>
#include <string>

namespace Ignosi::Modules::Render
{
    OpenGL_Shader::~OpenGL_Shader()
    {
        glDeleteShader(m_ID);
    }

    std::unique_ptr<Shader> OpenGL_Shader::Create(
        const std::filesystem::path&              shaderLoc,
        const std::vector<std::filesystem::path>& inc_paths)
    {
        std::string extension = shaderLoc.extension().string();
        if (extension == ".vert")
        {
            return Create(shaderLoc, ShaderType::Vertex, inc_paths);
        }
        if (extension == ".frag")
        {
            return Create(shaderLoc, ShaderType::Fragment, inc_paths);
        }
        if (extension == ".geom")
        {
            return Create(shaderLoc, ShaderType::Geometry, inc_paths);
        }
        if (extension == ".comp")
        {
            return Create(shaderLoc, ShaderType::Compute, inc_paths);
        }
        throw std::invalid_argument("Unkown shader extension " + extension);
    }

    std::unique_ptr<Shader> OpenGL_Shader::Create(
        const std::filesystem::path&              shaderLoc,
        ShaderType                                type,
        const std::vector<std::filesystem::path>& inc_paths)
    {
        if (!std::filesystem::is_regular_file(shaderLoc))
            throw std::invalid_argument(fmt::format("Shader file [{}] does not exist.", shaderLoc.string()));

        std::unique_ptr<OpenGL_Shader> pNewObj;

        switch (type)
        {
        case ShaderType::Fragment: pNewObj = std::make_unique<Fragment_OpenGL_Shader>(); break;
        case ShaderType::Vertex: pNewObj = std::make_unique<Vertex_OpenGL_Shader>(); break;
        case ShaderType::Compute: pNewObj = std::make_unique<Compute_OpenGL_Shader>(); break;
        case ShaderType::Geometry: pNewObj = std::make_unique<Geometry_OpenGL_Shader>(); break;
        default: throw std::invalid_argument(fmt::format("Unsupported Shader Type {}.", int(type)));
        }

        pNewObj->LoadShaderCode(shaderLoc);
        pNewObj->ID(pNewObj->CreateGLShader());
        pNewObj->Compile(inc_paths);

        return pNewObj;
    }

    void OpenGL_Shader::Compile(const std::vector<std::filesystem::path>& inc_paths)
    {
        Preprocessor processor;
        processor.IncludePaths(inc_paths);
        std::string processed_code  = processor.PreProcess(ShaderLoc(), ShaderCode());
        const char* pProcessed_code = processed_code.c_str();

        glShaderSource(m_ID, 1, &pProcessed_code, NULL);
        glCompileShader(m_ID);

        int success{0};
        glGetShaderiv(m_ID, GL_COMPILE_STATUS, &success);

        if (success == 0)
        {
            static constexpr size_t size = 1024;
            char                    infoLog[size];
            glGetShaderInfoLog(m_ID, size, NULL, infoLog);
            std::cout << infoLog << "\n\n";
            std::cout << "Code: \n" << processed_code << std::endl;

            throw std::runtime_error(fmt::format("Compilation failed, {}.", std::string(infoLog)));
        }
    }

} // namespace Ignosi::Modules::Render
