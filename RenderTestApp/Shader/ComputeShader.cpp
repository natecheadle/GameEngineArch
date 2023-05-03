#include "ComputeShader.h"

#include <glad/glad.h>

namespace nate::Modules::Render
{
    unsigned int ComputeShader::CreateGLShader()
    {
        return glCreateShader(GL_COMPUTE_SHADER);
    }
} // namespace nate::Modules::Render