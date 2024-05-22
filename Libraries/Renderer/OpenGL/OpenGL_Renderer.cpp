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

        // configure global opengl state
        // -----------------------------
        // TODO this should be user configurable.
        glEnable(GL_DEPTH_TEST);

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

    std::unique_ptr<IVertexBuffer> OpenGL_Renderer::CreateBuffer(const VertexDataConfig& config, std::span<const float> vertexes) const
    {
        return std::make_unique<OpenGL_VertexBuffer>(config, vertexes);
    }

    std::unique_ptr<IVertexBuffer> OpenGL_Renderer::CreateBuffer(
        const VertexDataConfig&        config,
        std::span<const float>         vertexes,
        std::span<const std::uint32_t> indeces) const
    {
        return std::make_unique<OpenGL_VertexBuffer>(config, vertexes, indeces);
    }

    std::unique_ptr<IShader> OpenGL_Renderer::CreateShader(
        const std::filesystem::path&              path,
        const std::vector<std::filesystem::path>& inc_paths) const
    {
        return OpenGL_Shader::Create(path, inc_paths);
    }

    std::unique_ptr<IShader> OpenGL_Renderer::CreateShader(
        const std::filesystem::path&              path,
        ShaderType                                type,
        const std::vector<std::filesystem::path>& inc_paths) const
    {
        return OpenGL_Shader::Create(path, type, inc_paths);
    }

    std::unique_ptr<IShaderProgram> OpenGL_Renderer::CreateShaderProgram(
        const IShader* pFragmentShader,
        const IShader* pGeometryShader,
        const IShader* pVertexShader) const
    {
        return std::make_unique<OpenGL_ShaderProgram>(pFragmentShader, pGeometryShader, pVertexShader);
    }

    std::unique_ptr<ITexture> OpenGL_Renderer::CreateTexture(const std::filesystem::path& path, TextureUnit unit) const
    {
        return std::make_unique<OpenGL_Texture>(path, unit);
    }

    std::unique_ptr<ITexture> OpenGL_Renderer::CreateTexture(const ImageFile& image, TextureUnit unit) const
    {
        return std::make_unique<OpenGL_Texture>(image, unit);
    }

    void OpenGL_Renderer::ClearDepthBuffer() const
    {
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void OpenGL_Renderer::ClearColorBuffer() const
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGL_Renderer::SwapBuffers() const
    {
        if (m_pWindow)
        {
            m_pWindow->SwapBuffers();
            m_pWindow->PollEvents();
        }
    }

    void OpenGL_Renderer::Update(std::chrono::milliseconds delta)
    {
        ClearDepthBuffer();
        ClearColorBuffer();

        for (const ECS::Component<Mesh>& mesh : ComponentPool())
        {
            // TODO - Material not yet supported
            // if (var.Material())
            // {
            //     var.Shader()->SetShaderVar("Material", *var.Material());
            // }
            mesh.Data().Shader()->Use();
            mesh.Data().Shader()->SetShaderVar("model", mesh.Data().ModelMatrix());

            if (m_pCamera)
            {
                mesh.Data().Shader()->SetShaderVar("view", m_pCamera->ViewPerspective());
                mesh.Data().Shader()->SetShaderVar("viewPos", m_pCamera->CameraPosition());
                mesh.Data().Shader()->SetShaderVar("projection", m_pCamera->Projection());
            }

            mesh.Data().Vertexes()->Draw();
        }

        SwapBuffers();
    }
} // namespace Ignosi::Libraries::Renderer
