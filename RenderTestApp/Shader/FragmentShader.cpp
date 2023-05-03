#include "FragmentShader.h"

#include <glad/glad.h>

namespace nate::Modules::Render
{
    unsigned int FragmentShader::CreateGLShader()
    {
        return glCreateShader(GL_FRAGMENT_SHADER);
    }
} // namespace nate::Modules::Render