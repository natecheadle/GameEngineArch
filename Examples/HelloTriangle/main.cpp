#include "Objects/Mesh.h"
#include "Shader/IShader.h"
#include "VertexDataConfig.h"

#include <IRenderer.h>
#include <IWindow.h>
#include <World.hpp>
#include <fmt/format.h>

#include <array>
#include <memory>

using namespace Ignosi::Libraries;
using namespace std::chrono_literals;

using TestWorld = ECS::World<Renderer::Mesh>;

const std::filesystem::path OutDir(OUT_DIR);

int main(int argc, char** argv)
{
    TestWorld world;

    std::unique_ptr<Renderer::IRenderer> pRendererUnique = Renderer::IRenderer::Create();
    Renderer::IRenderer*                 pRenderer       = world.Register<Renderer::Mesh>(std::move(pRendererUnique));
    Renderer::IWindow*                   pWindow         = pRenderer->AttachWindow(Renderer::IWindow::Create("test_window"));

    Renderer::VertexDataConfig vertexData;
    vertexData.PushBackConfig(Math::Vector3<float>());
    std::array<float, 9> vertices{-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

    ECS::ResourceManager&                   resourceManager = world.Resources();
    ECS::Resource<Renderer::IVertexBuffer>* pBuffer =
        resourceManager.CreateResource("TriangleVertexes", pRenderer->CreateBuffer(vertexData, vertices));

    ECS::Resource<Renderer::IShader>* pVertShader =
        resourceManager.CreateResource("VertexShader", pRenderer->CreateShader(OutDir / "assets/vert_shader.vert"));

    ECS::Resource<Renderer::IShader>* pFragShader =
        resourceManager.CreateResource("FragmentShader", pRenderer->CreateShader(OutDir / "assets/frag_shader.frag"));
    ECS::Resource<Renderer::IShaderProgram>* pShaderProgram =
        resourceManager.CreateResource("ShaderProgram", pRenderer->CreateShaderProgram(pFragShader->get(), nullptr, pVertShader->get()));

    auto pMesh = pRenderer->CreateComponent(Renderer::Mesh(pBuffer->get(), pShaderProgram->get(), nullptr));

    unsigned int              update = 0;
    std::chrono::milliseconds updatePeriod(1s / 60s);

    while (!pWindow->ShouldClose())
    {
        if (update % 60 == 0)
        {
            fmt::print("Window.Name: {} Window.Size: {}\n", pWindow->Name(), pWindow->ActualWindowSize());
        }

        world.Update(updatePeriod);
        update++;
    }

    // This is not explicitely necessary as the resource will be destroyed with the "World"
    resourceManager.DestroyResource("TriangleVertexes");
    resourceManager.DestroyResource("ShaderProgram");
    resourceManager.DestroyResource("VertexShader");
    resourceManager.DestroyResource("FragmentShader");
}
