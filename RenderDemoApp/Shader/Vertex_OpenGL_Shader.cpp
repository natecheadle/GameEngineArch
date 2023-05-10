#include "Vertex_OpenGL_Shader.h"

#include <glad/glad.h>

namespace nate::Modules::Render
{
    unsigned int Vertex_OpenGL_Shader::CreateGLShader()
    {
        return glCreateShader(GL_VERTEX_SHADER);
    }
} // namespace nate::Modules::Render
