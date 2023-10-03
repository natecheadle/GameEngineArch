#include "BreakOutApp.h"

#include "HitRectangle.h"
#include "IWindow.h"
#include "Renderer/Renderer.h"

#include <3D/Material.h>
#include <3D/Sprite.h>
#include <Keys.h>
#include <LinearAlgebra/Vector2.hpp>
#include <Renderer/Renderer_OpenGL.h>
#include <Texture/ImageFile.h>
#include <Units/Degree.hpp>

#include <memory>
#include <utility>

using namespace Ignosi::Modules;
using namespace std::chrono_literals;

namespace Ignosi::BreakOut
{
    BreakOutApp::BreakOutApp(std::unique_ptr<BreakOutWorld> pWorld, const GUI::WindowSize& window_size, std::string window_name)
        : App(std::move(pWorld))
    {
        m_pWorld = dynamic_cast<ECS::World<Render::Mesh3D, Render::Sprite, Physics::RigidBody2D, Physics::KinematicData>*>(App::GetWorld());
        m_pRenderer = m_pWorld->CreateSystem<Modules::Render::Renderer_OpenGL, Render::Mesh3D, Render::Sprite>();
        m_pPhysics  = m_pWorld->CreateSystem<Modules::Physics::PhysicsSystem, Physics::RigidBody2D, Physics::KinematicData>();
        m_pWindow   = m_pRenderer->InitializeWindow(window_size, std::move(window_name));
        m_pWorld->Window(m_pWindow);

        std::filesystem::path shader_dir(std::string_view(SHADER_DIR));
        m_LevelDir = shader_dir / "Levels";
    }

    Modules::GUI::IWindow* BreakOutApp::Initialize()
    {
        std::filesystem::path shader_dir(std::string_view(SHADER_DIR));
        std::filesystem::path shader_inc_dir = std::filesystem::path(std::string_view(SHADER_DIR));
        shader_dir /= "Shaders";
        shader_inc_dir /= "../../Modules/Render/Shader/GLSL/";
        assert(std::filesystem::is_directory(shader_dir));
        assert(std::filesystem::is_directory(shader_inc_dir));

        auto vertex_shader_path   = shader_dir / "vertex_shader.vert";
        auto fragment_shader_path = shader_dir / "fragment_shader.frag";
        auto background_path      = shader_dir / "background.jpg";
        auto paddle_path          = shader_dir / "paddle.png";
        auto ball_path            = shader_dir / "awesomeface.png";

        auto pBackMat   = std::make_unique<Render::Material>();
        auto pPaddleMat = std::make_shared<Render::Material>();
        auto pBallMat   = std::make_shared<Render::Material>();

        auto* pRenderer = m_pWorld->GetSystem<Render::Renderer>();

        Modules::Render::ImageFile background(background_path);
        pBackMat->Diffuse = pRenderer->CreateTexture("background", background, Modules::Render::TextureUnit::Texture0);

        Modules::Render::ImageFile paddle(paddle_path);
        pPaddleMat->Diffuse = pRenderer->CreateTexture("paddle", paddle, Render::TextureUnit::Texture0);

        Modules::Render::ImageFile ball(ball_path);
        pBallMat->Diffuse = pRenderer->CreateTexture("ball", ball, Render::TextureUnit::Texture0);

        auto* pVertexShader   = pRenderer->CreateShader(vertex_shader_path, {shader_inc_dir});
        auto* pFragmentShader = pRenderer->CreateShader(fragment_shader_path, {shader_inc_dir});
        m_pShader             = pRenderer->CreateShaderProgram(pFragmentShader, nullptr, pVertexShader);

        auto pCamera = std::make_unique<Render::Camera>(m_pWindow);
        pCamera->Near(-1.0);
        pCamera->Far(1.0);
        m_pRenderer->AttachedCamera(std::move(pCamera));

        float winWidth   = static_cast<float>(m_pWindow->GetLastWindowSize().Width());
        float windHeight = static_cast<float>(m_pWindow->GetLastWindowSize().Height());

        m_pBackground = std::make_unique<Background>(m_pWorld->CreateEntity(), pRenderer, static_cast<float>(background.AspectRatio()));
        m_pBackground->Sprite()->AttachedMaterial((std::move(pBackMat)));
        m_pBackground->KinematicData()->Position({0.0f, 0.0f});
        m_pBackground->Sprite()->Size({winWidth, windHeight});
        m_pBackground->Sprite()->Shader(m_pShader);

        m_pLevel = std::make_unique<Level>(m_pWorld, pRenderer);
        m_pLevel->Load(m_LevelDir / "One.lvl", static_cast<unsigned int>(winWidth), static_cast<unsigned int>(windHeight) / 2U, m_pShader);

        Vector2<float> paddleSize{100.0f, 20.0f};
        Vector2<float> playerPos{winWidth / 2.0f - paddleSize[0] / 2.0f, windHeight - paddleSize[1]};

        m_pPaddle = std::make_unique<Paddle>(m_pWorld->CreateEntity(), pRenderer, static_cast<float>(paddle.AspectRatio()));
        m_pPaddle->Sprite()->Size(paddleSize);
        m_pPaddle->Sprite()->AttachedMaterial(std::move(pPaddleMat));
        m_pPaddle->KinematicData()->Position(playerPos);
        m_pPaddle->Body()->HitShape(
            std::make_unique<Modules::Physics::HitRectangle>(m_pPaddle->KinematicData(), paddleSize[0], paddleSize[1]));
        m_pPaddle->Sprite()->Shader(m_pShader);
        m_pPaddle->PaddleMoveSpeed(PADDLE_SPEED);

        m_pBall = std::make_unique<Ball>(m_pWorld->CreateEntity(), pRenderer);
        m_pBall->Sprite()->AttachedMaterial(std::move(pBallMat));
        m_pBall->Sprite()->Shader(m_pShader);

        Vector2<float> ballPos = playerPos + Vector2<float>({paddleSize[0] / 2.0f - m_pBall->Radius(), -m_pBall->Radius() * 2.0f});
        m_pBall->KinematicData()->Position(ballPos);
        m_pBall->WindowWidth(winWidth);

        return m_pWindow;
    }

    void BreakOutApp::Shutdown()
    {
        m_pLevel.reset();
        m_pBackground.reset();
        m_pShader.reset();
        m_pPaddle.reset();
        m_pBall.reset();
    }

    void BreakOutApp::UpdateApp(double /* time */)
    {
        m_pWindow->ExecuteWithKeyStates([this](const GUI::KeyStateMap& keyStates) {
            if ((keyStates[GUI::Key::Space].first == GUI::KeyState::Pressed || keyStates[GUI::Key::Space].first == GUI::KeyState::Repeat))
            {
                m_pBall->Release();
            }
        });
    }
} // namespace Ignosi::BreakOut
