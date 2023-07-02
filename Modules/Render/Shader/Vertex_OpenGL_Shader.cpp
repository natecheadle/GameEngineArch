#include "Vertex_OpenGL_Shader.h"

#include <glad/glad.h>

namespace Ignosi::Modules::Render
{
    unsigned int Vertex_OpenGL_Shader::CreateGLShader()
    {
        return glCreateShader(GL_VERTEX_SHADER);
    }
} // namespace Ignosi::Modules::Render
