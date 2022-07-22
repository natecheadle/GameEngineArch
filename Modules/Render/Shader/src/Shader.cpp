#include "Shader.h"

#include <fmt/format.h>

#include <cstdint>
#include <filesystem>
#include <stdexcept>

namespace nate::Modules::Render {

    Shader::Shader(std::string_view name, std::filesystem::path shadersPath)
        : m_Path(GetFullPath(name, std::move(shadersPath)))
        , m_Handle(CreateHandle(m_Path))
    {
    }
    Shader::~Shader() {}

    bgfx::ShaderHandle Shader::CreateHandle(const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path))
            throw std::invalid_argument(fmt::format("Shader file does not exist at path {}.", path.string()));

        std::uintmax_t size = std::filesystem::file_size(path);
        std::ifstream  file(path, std::ios::binary);

        const bgfx::Memory* mem = bgfx::alloc(size + 1);
        file.read((char*)mem->data, size);
        mem->data[mem->size - 1] = '\0';

        file.close();

        return bgfx::createShader(mem);
    }

    std::filesystem::path Shader::GetFullPath(std::string_view name, std::filesystem::path shadersPath)
    {
        if (!std::filesystem::is_directory(shadersPath))
            throw std::invalid_argument(
                fmt::format("Path to shaders ({}) must be a path to a directory.", shadersPath.string()));

        bgfx::RendererType::Enum type = bgfx::getRendererType();
        switch (type)
        {
        case bgfx::RendererType::Direct3D9: shadersPath /= "dx9"; break;
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: shadersPath /= "dx11"; break;
        case bgfx::RendererType::Metal: shadersPath /= "metal"; break;
        case bgfx::RendererType::OpenGL: shadersPath /= "glsl"; break;
        case bgfx::RendererType::OpenGLES: shadersPath /= "essl"; break;
        case bgfx::RendererType::Vulkan: shadersPath /= "spirv"; break;
        default: throw std::invalid_argument(fmt::format("Render Type {} not supported.", (int)type));
        }

        shadersPath /= name;

        return std::filesystem::absolute(shadersPath);
    }
} // namespace nate::Modules::Render