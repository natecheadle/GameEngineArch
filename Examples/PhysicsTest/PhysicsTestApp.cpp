#include "PhysicsTestApp.h"

#include "Ball.h"
#include "RigidBody2D.h"

#include <Renderer/Renderer_OpenGL.h>

#include <chrono>
#include <filesystem>
#include <memory>
namespace Ignosi::Examples::PhysicsTest
{
    PhysicsTestApp::PhysicsTestApp(
        std::unique_ptr<ECS::World<Render::Mesh3D, Render::Sprite, Physics::RigidBody2D>> pWorld)
        : App::App(
              pWorld->CreateSystem<Render::Renderer_OpenGL, Render::Mesh3D, Render::Sprite>(),
              WINDOW_SIZE,
              std::string(WINDOW_NAME))
        , m_pWorld(std::move(pWorld))
    {
    }

    void PhysicsTestApp::Initialize()
    {
        std::filesystem::path resource_dir(std::string_view(RESOURCE_DIR));
        std::filesystem::path sprite_dir     = resource_dir / "Sprites";
        std::filesystem::path shader_dir     = resource_dir / "Shaders";
        std::filesystem::path shader_inc_dir = resource_dir / "../../../Modules/Render/Shader/GLSL/";

        auto vertex_shader_path   = shader_dir / "vertex_shader.vert";
        auto fragment_shader_path = shader_dir / "fragment_shader.frag";
        auto wall_path            = sprite_dir / "wall.jpg";
        auto ball_path            = sprite_dir / "ball.png";

        auto pWallMat = std::make_shared<Render::Material>();
        auto pBallMat = std::make_shared<Render::Material>();

        auto* pRenderer = GetRenderer();

        Modules::Render::ImageFile wall(wall_path);
        pWallMat->Diffuse = pRenderer->CreateTexture(wall, Modules::Render::TextureUnit::Texture0);

        Modules::Render::ImageFile ball(ball_path);
        pBallMat->Diffuse = pRenderer->CreateTexture(ball, Render::TextureUnit::Texture0);

        auto pVertexShader   = pRenderer->CreateShader(vertex_shader_path, {shader_inc_dir});
        auto pFragmentShader = pRenderer->CreateShader(fragment_shader_path, {shader_inc_dir});
        m_pShader            = pRenderer->CreateShaderProgram(pFragmentShader.get(), nullptr, pVertexShader.get());

        m_pCamera = std::make_unique<Render::Camera>(GetWindow());
        m_pCamera->Near(-1.0);
        m_pCamera->Far(1.0);

        float winWidth   = static_cast<float>(GetWindow()->GetLastWindowSize().Width());
        float windHeight = static_cast<float>(GetWindow()->GetLastWindowSize().Height());

        float wallWidth = 50.0f;

        m_pTopWall =
            std::make_unique<Wall>(m_pWorld->CreateEntity<Wall>(Render::Sprite(pRenderer), Physics::RigidBody2D()));
        m_pTopWall->Sprite().AttachedMaterial(pWallMat);
        m_pTopWall->Sprite().Origin({0.0, 0.0f});
        m_pTopWall->Sprite().Size({winWidth, wallWidth});

        m_pBottomWall =
            std::make_unique<Wall>(m_pWorld->CreateEntity<Wall>(Render::Sprite(pRenderer), Physics::RigidBody2D()));
        m_pBottomWall->Sprite().AttachedMaterial(pWallMat);
        m_pBottomWall->Sprite().Origin({0.0, windHeight - wallWidth});
        m_pBottomWall->Sprite().Size({winWidth, wallWidth});

        m_pLeftWall =
            std::make_unique<Wall>(m_pWorld->CreateEntity<Wall>(Render::Sprite(pRenderer), Physics::RigidBody2D()));
        m_pLeftWall->Sprite().AttachedMaterial(pWallMat);
        m_pLeftWall->Sprite().Origin({0.0f, 0.0});
        m_pLeftWall->Sprite().Size({wallWidth, windHeight});

        m_pRightWall =
            std::make_unique<Wall>(m_pWorld->CreateEntity<Wall>(Render::Sprite(pRenderer), Physics::RigidBody2D()));
        m_pRightWall->Sprite().AttachedMaterial(pWallMat);
        m_pRightWall->Sprite().Origin({winWidth - wallWidth, 0.0f});
        m_pRightWall->Sprite().Size({wallWidth, windHeight});

        m_pBall =
            std::make_unique<Ball>(m_pWorld->CreateEntity<Ball>(Render::Sprite(pRenderer), Physics::RigidBody2D()));
        m_pBall->Sprite().AttachedMaterial(pBallMat);
        m_pBall->Sprite().Origin({winWidth / 2.0f, windHeight / 2.0f});
        m_pBall->Sprite().Size({40.0f, 40.0f});
    }
    void PhysicsTestApp::UpdateApp(double dt)
    {
        m_pCamera->Update(std::chrono::nanoseconds(static_cast<unsigned long long>(dt * 1e-9)));

        auto renderUpdate = [&]() -> void {
            m_pShader->Use();
            m_pShader->SetShaderVar("projection", m_pCamera->ViewOrthographic());

            GetRenderer()->DrawAllSprites(m_pShader.get());
        };

        GetRenderer()->ExecuteFunction(renderUpdate);
    }
    void PhysicsTestApp::Shutdown()
    {
        m_pBall.reset();
        m_pTopWall.reset();
        m_pBottomWall.reset();
        m_pLeftWall.reset();
        m_pRightWall.reset();
        m_pShader.reset();
        m_pCamera.reset();
        m_pWorld.reset();
    }
} // namespace Ignosi::Examples::PhysicsTest