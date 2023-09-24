#include "Fragment_OpenGL_Shader.h"

#include <glad/glad.h>

namespace Ignosi::Modules::Render
{
    Fragment_OpenGL_Shader::Fragment_OpenGL_Shader(const std::filesystem::path& shaderLoc)
        : OpenGL_Shader(shaderLoc)
    {
    }

    unsigned int Fragment_OpenGL_Shader::CreateGLShader()
    {
        return glCreateShader(GL_FRAGMENT_SHADER);
    }
} // namespace Ignosi::Modules::Render
