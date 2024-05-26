#include "HelloCubeApp.h"

#include <Camera/Fly_Camera.h>
#include <Entity.hpp>
#include <IRenderer.h>
#include <IWindow.h>
#include <Objects/Mesh.h>
#include <Shader/IShader.h>
#include <Texture/Material.h>
#include <VertexDataConfig.h>
#include <World.hpp>
#include <fmt/format.h>

#include <memory>

namespace Ignosi::Example
{
    namespace
    {
        const std::filesystem::path OutDir(OUT_DIR);
    } // namespace

    bool HelloCubeApp::KeepUpdating()
    {
        return m_pWindow && !m_pWindow->ShouldClose();
    }

    void HelloCubeApp::Initialize()
    {
        m_Entity = m_World.CreateEntity();

        std::unique_ptr<Libraries::Renderer::IRenderer> pRendererUnique = Libraries::Renderer::IRenderer::Create();
        Libraries::Renderer::IRenderer*                 pRenderer = m_World.Register<Libraries::Renderer::Mesh>(std::move(pRendererUnique));
        m_pWindow = pRenderer->AttachWindow(Libraries::Renderer::IWindow::Create("test_window"));

        Libraries::Renderer::VertexDataConfig vertexData;
        vertexData.PushBackConfig(Libraries::Math::Vector3<float>());

        Libraries::ECS::ResourceManager&                              resourceManager = m_World.Resources();
        Libraries::ECS::Resource<Libraries::Renderer::IVertexBuffer>* pBuffer =
            resourceManager.CreateResource("CubeVertexes", Libraries::Renderer::Mesh::CreateCubeVertexes(pRenderer));

        Libraries::ECS::Resource<Libraries::Renderer::IShader>* pVertShader =
            resourceManager.CreateResource("VertexShader", pRenderer->CreateShader(OutDir / "assets/vert_shader.vert"));

        Libraries::ECS::Resource<Libraries::Renderer::IShader>* pFragShader =
            resourceManager.CreateResource("FragmentShader", pRenderer->CreateShader(OutDir / "assets/frag_shader.frag"));
        Libraries::ECS::Resource<Libraries::Renderer::IShaderProgram>* pShaderProgram = resourceManager.CreateResource(
            "ShaderProgram",
            pRenderer->CreateShaderProgram(pFragShader->get(), nullptr, pVertShader->get()));

        Libraries::ECS::Resource<Libraries::Renderer::ITexture>* pTexture = resourceManager.CreateResource(
            "Texture",
            pRenderer->CreateTexture(OutDir / "assets/container.jpg", Libraries::Renderer::TextureUnit::Texture0));
        Libraries::ECS::Resource<Libraries::Renderer::IMaterial>* pMaterial = resourceManager.CreateResource(
            "Material",
            std::unique_ptr<Libraries::Renderer::IMaterial>(std::make_unique<Libraries::Renderer::Material>()));

        pMaterial->get()->Diffuse(pTexture->get());

        auto pCamera = std::make_unique<Libraries::Renderer::Fly_Camera>(m_pWindow);
        pCamera->PanLeft(1.0);
        pCamera->PanUp(1.0);

        pRenderer->AttachedCamera(std::move(pCamera));

        m_Entity->Set(Libraries::Renderer::Mesh(pBuffer->get(), pShaderProgram->get(), pMaterial->get()));
    }

    void HelloCubeApp::Update()
    {
        m_World.Update(m_UpdatePeriod);
        m_Update++;
    }
    void HelloCubeApp::ShutDown() {}
} // namespace Ignosi::Example
