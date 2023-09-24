#include "Compute_OpenGL_Shader.h"

#include <glad/glad.h>

namespace Ignosi::Modules::Render
{
    Compute_OpenGL_Shader::Compute_OpenGL_Shader(const std::filesystem::path& shaderLoc)
        : OpenGL_Shader(shaderLoc)
    {
    }

    unsigned int Compute_OpenGL_Shader::CreateGLShader()
    {
        return glCreateShader(GL_COMPUTE_SHADER);
    }
} // namespace Ignosi::Modules::Render
