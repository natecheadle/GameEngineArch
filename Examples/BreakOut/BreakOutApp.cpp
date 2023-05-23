#include "BreakOutApp.h"

#include "3D/Sprite.h"
#include "Keys.h"

#include <Renderer/Renderer_OpenGL.h>
#include <Units/Degree.hpp>

#include <memory>

using namespace nate::Modules;
using namespace std::chrono_literals;

namespace nate::BreakOut
{
    BreakOutApp::BreakOutApp(
        std::unique_ptr<ECS::World<Render::Mesh3D, Render::Sprite>> pWorld,
        const GUI::WindowSize&                                      window_size,
        std::string                                                 window_name)
        : App(pWorld->CreateSystem<Render::Renderer_OpenGL, Render::Mesh3D, Render::Sprite>(),
              window_size,
              std::move(window_name))
        , m_pWorld(std::move(pWorld))
    {
        std::filesystem::path shader_dir(std::string_view(SHADER_DIR));
        m_LevelDir = shader_dir / "Levels";
    }

    void BreakOutApp::Initialize()
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

        auto backMaterial = std::make_unique<Render::Material>();
        auto pPaddleMat   = std::make_shared<Render::Material>();

        auto* pRenderer = GetRenderer();

        backMaterial->Diffuse = pRenderer->CreateTexture(background_path, nate::Modules::Render::TextureUnit::Texture0);

        pPaddleMat->Diffuse = pRenderer->CreateTexture(paddle_path, Render::TextureUnit::Texture0);

        auto pVertexShader   = pRenderer->CreateShader(vertex_shader_path, {shader_inc_dir});
        auto pFragmentShader = pRenderer->CreateShader(fragment_shader_path, {shader_inc_dir});
        m_pShader            = pRenderer->CreateShaderProgram(pFragmentShader.get(), nullptr, pVertexShader.get());

        m_pCamera = std::make_unique<Render::Camera>(GetWindow());
        m_pCamera->Near(-1.0);
        m_pCamera->Far(1.0);

        float winWidth   = static_cast<float>(GetWindow()->GetLastWindowSize().Width());
        float windHeight = static_cast<float>(GetWindow()->GetLastWindowSize().Height());

        m_pBackground =
            std::make_unique<Background>(std::move(m_pWorld->CreateEntity<Background>(Render::Sprite(pRenderer))));
        m_pBackground->Sprite().AttachedMaterial((std::move(backMaterial)));
        m_pBackground->Sprite().Origin({0.0f, 0.0});
        m_pBackground->Sprite().Size({winWidth, windHeight * 2});
        m_pBackground->Sprite().Color({1.0f, 1.0f, 1.0f});

        m_pLevel = std::make_unique<Level>(m_pWorld.get(), pRenderer);
        m_pLevel->Load(
            m_LevelDir / "One.lvl",
            static_cast<unsigned int>(winWidth),
            static_cast<unsigned int>(windHeight) / 2U);

        Vector2<float> paddleSize{100.0f, 100.0f};
        m_pPaddle = std::make_unique<Paddle>(std::move(m_pWorld->CreateEntity<Paddle>(Render::Sprite(pRenderer))));
        m_pPaddle->Sprite().AttachedMaterial(std::move(pPaddleMat));
        m_pPaddle->Sprite().Size(paddleSize);
        m_pPaddle->Sprite().Origin({winWidth / 2.0f - paddleSize[0] / 2.0f, windHeight - paddleSize[1] / 4});
        m_pPaddle->Sprite().Color({1.0f, 1.0f, 1.0f});
    }

    void BreakOutApp::Shutdown()
    {
        m_pCamera.reset();
        m_pLevel.reset();
        m_pBackground.reset();
        m_pShader.reset();
        m_pPaddle.reset();
    }

    void BreakOutApp::UpdateApp(std::chrono::nanoseconds time)
    {
        // TODO this should be handled automatically
        m_pCamera->Update(time);

        GetWindow()->ExecuteWithKeyStates([this](const GUI::KeyStateMap& keyStates) {
            if ((keyStates[GUI::Key::Left].first == GUI::KeyState::Pressed ||
                 keyStates[GUI::Key::Left].first == GUI::KeyState::Repeat))
            {
                if (m_pPaddle->Sprite().Origin()[0] > 0.0)
                {
                    m_pPaddle->Sprite().TranslateX(-PADDLE_SPEED);
                }
            }

            if ((keyStates[GUI::Key::Right].first == GUI::KeyState::Pressed ||
                 keyStates[GUI::Key::Right].first == GUI::KeyState::Repeat))
            {
                if (m_pPaddle->Sprite().Origin()[0] <
                    static_cast<float>(GetWindow()->GetLastWindowSize().Width()) - m_pPaddle->Sprite().Size()[0])
                {
                    m_pPaddle->Sprite().TranslateX(PADDLE_SPEED);
                }
            }
        });

        auto renderUpdate = [&]() -> void {
            m_pShader->Use();
            m_pShader->SetShaderVar("projection", m_pCamera->ViewOrthographic());

            GetRenderer()->DrawAllSprites(m_pShader.get());
        };

        GetRenderer()->ExecuteFunction(renderUpdate);
    }
} // namespace nate::BreakOut
