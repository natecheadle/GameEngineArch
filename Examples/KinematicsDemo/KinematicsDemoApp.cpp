#include "KinematicsDemoApp.h"

#include <IPhysicsEngine.h>
#include <IRenderer.h>
#include <IWindow.h>
#include <PhysicsData.h>
#include <PhysicsEngine.h>
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

    bool KinematicsDemoApp::KeepUpdating()
    {
        return m_pWindow && !m_pWindow->ShouldClose();
    }

    void KinematicsDemoApp::Initialize()
    {
        m_Entity = m_World.CreateEntity();

        std::unique_ptr<Libraries::Renderer::IRenderer> pRendererUnique = Libraries::Renderer::IRenderer::Create();
        Libraries::Renderer::IRenderer*                 pRenderer = m_World.Register<Libraries::Renderer::Mesh>(std::move(pRendererUnique));
        m_pWindow = pRenderer->AttachWindow(Libraries::Renderer::IWindow::Create("test_window"));

        std::unique_ptr<Libraries::Physics::IPhysicsEngine> pPhysicsUnique = std::make_unique<Libraries::Physics::PhysicsEngine>();
        Libraries::Physics::IPhysicsEngine* pPhysics = m_World.Register<Libraries::Physics::PhysicsData>(std::move(pPhysicsUnique));

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

    void KinematicsDemoApp::Update()
    {
        m_World.Update(m_UpdatePeriod);
        m_Update++;
    }

    void KinematicsDemoApp::ShutDown() {}
} // namespace Ignosi::Example
