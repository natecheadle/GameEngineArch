#include "OpenGL_Shader.h"

#include "Compute_OpenGL_Shader.h"
#include "Fragment_OpenGL_Shader.h"
#include "Geometry_OpenGL_Shader.h"
#include "Vertex_OpenGL_Shader.h"

#include <fmt/format.h>
#include <glad/glad.h>

#include <stdexcept>
#include <string>

namespace nate::Modules::Render
{
    OpenGL_Shader::~OpenGL_Shader()
    {
        glDeleteShader(m_ID);
    }

    std::unique_ptr<Shader> OpenGL_Shader::Create(const std::filesystem::path& shaderLoc)
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

    std::unique_ptr<Shader> OpenGL_Shader::Create(const std::filesystem::path& shaderLoc, ShaderType type)
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
        pNewObj->Compile();

        return pNewObj;
    }

    void OpenGL_Shader::Compile()
    {
        const char* pStr = ShaderCode().c_str();

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

} // namespace nate::Modules::Render
