#pragma once

#include "Background.h"
#include "Ball.h"
#include "Level.h"
#include "Shader/ShaderProgram.h"

#include <3D/Camera.h>
#include <3D/Mesh3D.h>
#include <3D/Sprite.h>
#include <App.h>

#include <filesystem>
#include <memory>

namespace Ignosi::BreakOut
{
    class BreakOutApp : public Modules::App::App
    {

        std::unique_ptr<Modules::Render::Camera>         m_pCamera;
        std::unique_ptr<Modules::Render::ShaderProgram>  m_pShader;
        std::unique_ptr<BreakOutWorld>                   m_pWorld;
        std::unique_ptr<Background>                      m_pBackground;
        std::unique_ptr<Level>                           m_pLevel;
        std::unique_ptr<Paddle>                          m_pPaddle;
        std::unique_ptr<Ball>                            m_pBall;
        std::filesystem::path                            m_LevelDir;
        std::unique_ptr<Modules::Physics::PhysicsSystem> m_pPhysicsSys;

        static constexpr float PADDLE_SPEED = 6.0f; // pixels per update

      public:
        BreakOutApp(std::unique_ptr<BreakOutWorld> pWorld, const Modules::GUI::WindowSize& window_size, std::string window_name);

      protected:
        Modules::GUI::IWindow* Initialize() override;
        void                   Shutdown() override;
        void                   UpdateApp(double time) override;
    };
} // namespace Ignosi::BreakOut
