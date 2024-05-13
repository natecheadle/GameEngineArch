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
    TestWorld m_World;

    std::unique_ptr<Renderer::IRenderer> pRendererUnique = Renderer::IRenderer::Create();
    Renderer::IRenderer*                 pRenderer       = m_World.Register<Renderer::Mesh>(std::move(pRendererUnique));
    Renderer::IWindow*                   pWindow         = pRenderer->AttachWindow(Renderer::IWindow::Create("test_window"));

    Renderer::VertexDataConfig vertexData;
    vertexData.PushBackConfig(Math::Vector3<float>());
    std::array<float, 9> vertices{-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

    std::unique_ptr<Renderer::IVertexBuffer>  pBuffer     = pRenderer->CreateBuffer(vertexData, vertices);
    std::unique_ptr<Renderer::IShader>        pVertShader = pRenderer->CreateShader(OutDir / "assets/vert_shader.vert");
    std::unique_ptr<Renderer::IShader>        pFragShader = pRenderer->CreateShader(OutDir / "assets/frag_shader.frag");
    std::unique_ptr<Renderer::IShaderProgram> pShaderProgram =
        pRenderer->CreateShaderProgram(std::move(pFragShader), nullptr, std::move(pVertShader));

    auto pMesh = pRenderer->CreateComponent(Renderer::Mesh(std::move(pBuffer), std::move(pShaderProgram), nullptr));

    unsigned int              update = 0;
    std::chrono::milliseconds updatePeriod(1s / 60s);

    while (!pWindow->ShouldClose())
    {
        if (update % 60 == 0)
        {
            fmt::print("Window.Name: {} Window.Size: {}\n", pWindow->Name(), pWindow->ActualWindowSize());
        }

        m_World.Update(updatePeriod);
        update++;
    }
}
