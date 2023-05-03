#include "VertexShader.h"

#include <glad/glad.h>

namespace nate::Modules::Render
{
    unsigned int VertexShader::CreateGLShader()
    {
        return glCreateShader(GL_VERTEX_SHADER);
    }
} // namespace nate::Modules::Render
