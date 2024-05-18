#include "HelloTriangleApp.h"

#include "Entity.hpp"
#include "Objects/Mesh.h"
#include "Shader/IShader.h"
#include "VertexDataConfig.h"

#include <IRenderer.h>
#include <IWindow.h>
#include <World.hpp>
#include <fmt/format.h>

#include <array>
#include <memory>

namespace Ignosi::Example
{
    namespace
    {
        const std::filesystem::path OutDir(OUT_DIR);
    }

    bool HelloTriangleApp::KeepUpdating()
    {
        return m_pWindow && !m_pWindow->ShouldClose();
    }

    void HelloTriangleApp::Initialize()
    {
        m_Entity = m_World.CreateEntity();

        std::unique_ptr<Libraries::Renderer::IRenderer> pRendererUnique = Libraries::Renderer::IRenderer::Create();
        Libraries::Renderer::IRenderer*                 pRenderer = m_World.Register<Libraries::Renderer::Mesh>(std::move(pRendererUnique));
        m_pWindow = pRenderer->AttachWindow(Libraries::Renderer::IWindow::Create("test_window"));

        Libraries::Renderer::VertexDataConfig vertexData;
        vertexData.PushBackConfig(Libraries::Math::Vector3<float>());
        std::array<float, 9> vertices{-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

        Libraries::ECS::ResourceManager&                              resourceManager = m_World.Resources();
        Libraries::ECS::Resource<Libraries::Renderer::IVertexBuffer>* pBuffer =
            resourceManager.CreateResource("TriangleVertexes", pRenderer->CreateBuffer(vertexData, vertices));

        Libraries::ECS::Resource<Libraries::Renderer::IShader>* pVertShader =
            resourceManager.CreateResource("VertexShader", pRenderer->CreateShader(OutDir / "assets/vert_shader.vert"));

        Libraries::ECS::Resource<Libraries::Renderer::IShader>* pFragShader =
            resourceManager.CreateResource("FragmentShader", pRenderer->CreateShader(OutDir / "assets/frag_shader.frag"));
        Libraries::ECS::Resource<Libraries::Renderer::IShaderProgram>* pShaderProgram = resourceManager.CreateResource(
            "ShaderProgram",
            pRenderer->CreateShaderProgram(pFragShader->get(), nullptr, pVertShader->get()));

        m_Entity->Set(Libraries::Renderer::Mesh(pBuffer->get(), pShaderProgram->get(), nullptr));
    }
    void HelloTriangleApp::Update()
    {
        if (m_Update % 60 == 0)
        {
            fmt::print("Window.Name: {} Window.Size: {}\n", m_pWindow->Name(), m_pWindow->ActualWindowSize());
        }

        m_World.Update(m_UpdatePeriod);
        m_Update++;
    }
    void HelloTriangleApp::ShutDown() {}
} // namespace Ignosi::Example
