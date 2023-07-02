#include "Fragment_OpenGL_Shader.h"

#include <glad/glad.h>

namespace Ignosi::Modules::Render
{
    unsigned int Fragment_OpenGL_Shader::CreateGLShader()
    {
        return glCreateShader(GL_FRAGMENT_SHADER);
    }
} // namespace Ignosi::Modules::Render