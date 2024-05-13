#include "Vertex_OpenGL_Shader.h"

#include <glad/glad.h>

namespace Ignosi::Libraries::Renderer
{
    Vertex_OpenGL_Shader::Vertex_OpenGL_Shader(const std::filesystem::path& shaderLoc)
        : OpenGL_Shader(shaderLoc)
    {
    }

    unsigned int Vertex_OpenGL_Shader::CreateGLShader()
    {
        return glCreateShader(GL_VERTEX_SHADER);
    }
} // namespace Ignosi::Modules::Render
