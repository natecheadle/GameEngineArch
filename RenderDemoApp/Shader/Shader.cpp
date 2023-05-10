#include "Shader.h"

#include <fmt/format.h>

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace nate::Modules::Render
{
    void Shader::LoadShaderCode(const std::filesystem::path& shaderLoc)
    {
        std::ifstream shaderFile(shaderLoc);
        if (!shaderFile.is_open())
            throw std::invalid_argument(fmt::format("Shader file [{}] does not exist.", shaderLoc.string()));

        std::stringstream sstream;
        sstream << shaderFile.rdbuf();
        m_ShaderCode = sstream.str();
    }
} // namespace nate::Modules::Render
