#include "Compute_OpenGL_Shader.h"

#include <glad/glad.h>

namespace nate::Modules::Render
{
    unsigned int Compute_OpenGL_Shader::CreateGLShader()
    {
        return glCreateShader(GL_COMPUTE_SHADER);
    }
} // namespace nate::Modules::Render