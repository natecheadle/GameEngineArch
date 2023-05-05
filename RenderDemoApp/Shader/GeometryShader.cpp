#include "GeometryShader.h"

#include <glad/glad.h>

namespace nate::Modules::Render
{
    unsigned int GeometryShader::CreateGLShader()
    {
        return glCreateShader(GL_GEOMETRY_SHADER);
    }
} // namespace nate::Modules::Render