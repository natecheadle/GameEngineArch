#include "Geometry_OpenGL_Shader.h"

#include <glad/glad.h>

namespace nate::Modules::Render
{
    unsigned int Geometry_OpenGL_Shader::CreateGLShader()
    {
        return glCreateShader(GL_GEOMETRY_SHADER);
    }
} // namespace nate::Modules::Render