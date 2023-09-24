#include "Geometry_OpenGL_Shader.h"

#include <glad/glad.h>

namespace Ignosi::Modules::Render
{
    Geometry_OpenGL_Shader::Geometry_OpenGL_Shader(const std::filesystem::path& shaderLoc)
        : OpenGL_Shader(shaderLoc)
    {
    }

    unsigned int Geometry_OpenGL_Shader::CreateGLShader()
    {
        return glCreateShader(GL_GEOMETRY_SHADER);
    }
} // namespace Ignosi::Modules::Render
