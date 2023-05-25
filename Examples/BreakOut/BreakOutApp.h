#pragma once

#include "Background.h"
#include "Ball.h"
#include "BreakOutWorld.h"
#include "Level.h"

#include <3D/Camera.h>
#include <3D/Mesh3D.h>
#include <3D/Sprite.h>
#include <App.h>

#include <filesystem>
#include <memory>

namespace nate::BreakOut
{
    class BreakOutApp : public Modules::App::App
    {

        std::unique_ptr<Modules::Render::Camera> m_pCamera;
        Modules::Render::ShaderProgram_ptr       m_pShader;
        std::unique_ptr<BreakOutWorld>           m_pWorld;
        std::unique_ptr<Background>              m_pBackground;
        std::unique_ptr<Level>                   m_pLevel;
        std::unique_ptr<Paddle>                  m_pPaddle;
        std::unique_ptr<Ball>                    m_pBall;
        std::filesystem::path                    m_LevelDir;

        static constexpr float PADDLE_SPEED = 3.0f; // pixels per update

      public:
        BreakOutApp(
            std::unique_ptr<BreakOutWorld>  pWorld,
            const Modules::GUI::WindowSize& window_size,
            std::string                     window_name);

      protected:
        void Initialize() override;
        void Shutdown() override;
        void UpdateApp(std::chrono::nanoseconds time) override;
    };
} // namespace nate::BreakOut
