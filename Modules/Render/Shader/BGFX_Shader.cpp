#include "BGFX_Shader.h"

#include "Renderer.h"

#include <fmt/format.h>

#include <cassert>

namespace nate::Modules::Render
{
    BGFX_Shader::BGFX_Shader(std::string_view name, std::filesystem::path shadersPath, Renderer* pRenderer)
        : Shader(GetFullPath(name, std::move(shadersPath)))
        , m_Handle(pRenderer->CreateShader(ShaderData()))
    {
    }

    std::filesystem::path BGFX_Shader::GetFullPath(std::string_view name, std::filesystem::path shadersPath)
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
