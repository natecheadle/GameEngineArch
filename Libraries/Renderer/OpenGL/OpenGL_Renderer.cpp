#include "OpenGL_Renderer.h"

#include "OpenGL/Shader/OpenGL_Shader.h"
#include "OpenGL/Shader/OpenGL_ShaderProgram.h"
#include "OpenGL/Texture/OpenGL_Texture.h"
#include "OpenGL_VertexBuffer.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <memory>

namespace Ignosi::Libraries::Renderer
{
    IWindow* OpenGL_Renderer::AttachedWindow() const
    {
        return m_pWindow.get();
    }

    IWindow* OpenGL_Renderer::AttachWindow(std::unique_ptr<IWindow> pWindow)
    {
        m_pWindow = std::move(pWindow);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
        {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        WindowSize size = m_pWindow->ActualWindowSize();
        glViewport(0, 0, size.Width, size.Height);

        return m_pWindow.get();
    }

    void OpenGL_Renderer::AttachedCamera(std::shared_ptr<Camera> camera)
    {
        m_pCamera = std::move(camera);
    }

    const std::shared_ptr<Camera>& OpenGL_Renderer::AttachedCamera() const
    {
        return m_pCamera;
    }

    std::unique_ptr<IVertexBuffer> OpenGL_Renderer::CreateBuffer(const VertexDataConfig& config, std::span<const float> vertexes)
    {
        return std::make_unique<OpenGL_VertexBuffer>(config, vertexes);
    }

    std::unique_ptr<IVertexBuffer> OpenGL_Renderer::CreateBuffer(
        const VertexDataConfig&        config,
        std::span<const float>         vertexes,
        std::span<const std::uint32_t> indeces)
    {
        return std::make_unique<OpenGL_VertexBuffer>(config, vertexes, indeces);
    }

    std::unique_ptr<IShader> OpenGL_Renderer::CreateShader(
        const std::filesystem::path&              path,
        const std::vector<std::filesystem::path>& inc_paths)
    {
        return OpenGL_Shader::Create(path, inc_paths);
    }

    std::unique_ptr<IShader> OpenGL_Renderer::CreateShader(
        const std::filesystem::path&              path,
        ShaderType                                type,
        const std::vector<std::filesystem::path>& inc_paths)
    {
        return OpenGL_Shader::Create(path, type, inc_paths);
    }

    std::unique_ptr<IShaderProgram> OpenGL_Renderer::CreateShaderProgram(
        std::shared_ptr<const IShader> pFragmentShader,
        std::shared_ptr<const IShader> pGeometryShader,
        std::shared_ptr<const IShader> pVertexShader)
    {
        return std::make_unique<OpenGL_ShaderProgram>(std::move(pFragmentShader), std::move(pGeometryShader), std::move(pVertexShader));
    }

    std::unique_ptr<ITexture> OpenGL_Renderer::CreateTexture(
        const std::string&           textureName,
        const std::filesystem::path& path,
        TextureUnit                  unit)
    {
        return std::make_unique<OpenGL_Texture>(textureName, path, unit);
    }

    std::unique_ptr<ITexture> OpenGL_Renderer::CreateTexture(const std::string& textureName, const ImageFile& image, TextureUnit unit)
    {
        return std::make_unique<OpenGL_Texture>(textureName, image, unit);
    }

    void OpenGL_Renderer::ClearDepthBuffer()
    {
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void OpenGL_Renderer::ClearColorBuffer()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGL_Renderer::SwapBuffers()
    {
        if (m_pWindow)
        {
            m_pWindow->SwapBuffers();
            m_pWindow->PollEvents();
        }
    }

    void OpenGL_Renderer::Update(std::chrono::milliseconds delta)
    {
        for (const Mesh& var : ComponentPool())
        {
            // TODO - Material not yet supported
            // if (var.Material())
            // {
            //     var.Shader()->SetShaderVar("Material", *var.Material());
            // }
            var.Shader()->Use();
            var.Vertexes()->Draw();
        }

        SwapBuffers();
    }
} // namespace Ignosi::Libraries::Renderer
