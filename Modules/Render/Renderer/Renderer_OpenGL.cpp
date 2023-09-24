#include "Renderer_OpenGL.h"

#include "../Shader/OpenGL_Shader.h"
#include "../Shader/OpenGL_ShaderProgram.h"
#include "../Texture/OpenGL_Texture.h"
#include "IWindow.h"
#include "Renderer/VertexBuffer.h"
#include "Shader/ShaderProgram.h"
#include "Tag.h"
#include "Texture/Texture.h"
#include "VertexBuffer_OpenGL.h"
#include "WindowMessages.hpp"

#include <DebugCast.hpp>
#include <Messages/WindowResized.hpp>
#include <Window_GLFW.h>
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace Ignosi::Modules::Render
{
    Renderer_OpenGL::Renderer_OpenGL(ECS::ComponentPool<Mesh3D>* pMeshPool, ECS::ComponentPool<Sprite>* pSpritePool)
        : Renderer(pMeshPool, pSpritePool)
    {
    }

    Renderer_OpenGL::~Renderer_OpenGL()
    {
        m_pWin->Unsubscribe(this);
        m_pWin.reset();
    }

    GUI::IWindow* Renderer_OpenGL::InitializeWindow(const GUI::WindowSize& size, std::string name)
    {
        // Mac requires the window to be in the main thread.
        m_pWin = std::make_unique<GUI::Window_GLFW>(size, std::move(name));

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
        {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        // configure global opengl state
        // -----------------------------
        // TODO this should be user configurable.
        glEnable(GL_DEPTH_TEST);

        return m_pWin.get();
    }

    std::unique_ptr<VertexBuffer> Renderer_OpenGL::CreateBuffer(const VertexDataConfig& config, std::span<const float> vertexes)
    {
        return std::make_unique<VertexBuffer_OpenGL>(config, vertexes);
    }

    std::unique_ptr<VertexBuffer> Renderer_OpenGL::CreateBuffer(
        const VertexDataConfig&        config,
        std::span<const float>         vertexes,
        std::span<const std::uint32_t> indeces)
    {
        return std::make_unique<VertexBuffer_OpenGL>(config, vertexes, indeces);
    }

    Shader* Renderer_OpenGL::CreateShader(const std::filesystem::path& path, const std::vector<std::filesystem::path>& inc_paths)
    {
        auto pResource = World()->Resources().GetResource(path.string());
        if (pResource)
        {
            return DebugCast<Shader*>(pResource);
        }
        return DebugCast<Shader*>(World()->Resources().LoadResource(OpenGL_Shader::Create(path, inc_paths)));
    }

    Shader* Renderer_OpenGL::CreateShader(
        const std::filesystem::path&              path,
        ShaderType                                type,
        const std::vector<std::filesystem::path>& inc_paths)
    {
        auto pResource = World()->Resources().GetResource(path.string());
        if (pResource)
        {
            return DebugCast<Shader*>(pResource);
        }
        return DebugCast<Shader*>(World()->Resources().LoadResource(OpenGL_Shader::Create(path, type, inc_paths)));
    }

    std::unique_ptr<ShaderProgram> Renderer_OpenGL::CreateShaderProgram(
        const Shader* pFragmentShader,
        const Shader* pGeometryShader,
        const Shader* pVertexShader)
    {
        return std::make_unique<OpenGL_ShaderProgram>(pFragmentShader, pGeometryShader, pVertexShader);
    }

    std::unique_ptr<Texture> Renderer_OpenGL::CreateTexture(
        const std::string&           textureName,
        const std::filesystem::path& path,
        TextureUnit                  unit)
    {
        return std::make_unique<OpenGL_Texture>(textureName, path, unit);
    }

    std::unique_ptr<Texture> Renderer_OpenGL::CreateTexture(const std::string& textureName, const ImageFile& image, TextureUnit unit)
    {
        return std::make_unique<OpenGL_Texture>(textureName, image, unit);
    }

    void Renderer_OpenGL::ClearDepthBuffer()
    {
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void Renderer_OpenGL::ClearColorBuffer()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer_OpenGL::SwapBuffers()
    {
        glfwSwapBuffers(this->m_pWin->GetGLFWWindow());
        m_pWin->PollEvents();
    }

    void Renderer_OpenGL::OnWindowResized(const GUI::WindowMessage* pMessage)
    {
        const auto* pWindowResized = DebugCast<const GUI::WindowResized*>(pMessage);
        glViewport(0, 0, pWindowResized->GetData()->Width(), pWindowResized->GetData()->Height());
    }
} // namespace Ignosi::Modules::Render
