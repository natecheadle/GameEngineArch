#pragma once

#include "Background.h"
#include "Ball.h"
#include "Level.h"

#include <3D/Camera.h>
#include <3D/Mesh3D.h>
#include <3D/Sprite.h>
#include <App.h>
#include <IWindow.h>
#include <PhysicsSystem.h>
#include <Renderer/Renderer.h>
#include <Shader/ShaderProgram.h>

#include <filesystem>
#include <memory>

namespace Ignosi::BreakOut
{
    class BreakOutApp : public Modules::App::App
    {

        std::shared_ptr<Modules::Render::ShaderProgram> m_pShader;
        BreakOutWorld*                                  m_pWorld;
        std::unique_ptr<Background>                     m_pBackground;
        std::unique_ptr<Level>                          m_pLevel;
        std::unique_ptr<Paddle>                         m_pPaddle;
        std::unique_ptr<Ball>                           m_pBall;
        std::filesystem::path                           m_LevelDir;
        Modules::Render::Renderer*                      m_pRenderer;
        Modules::Physics::PhysicsSystem*                m_pPhysics;
        Modules::GUI::IWindow*                          m_pWindow;

        static constexpr float PADDLE_SPEED = 360.0f; // pixels per second

      public:
        BreakOutApp(std::unique_ptr<BreakOutWorld> pWorld, const Modules::GUI::WindowSize& window_size, std::string window_name);

      protected:
        Modules::GUI::IWindow* Initialize() override;
        void                   Shutdown() override;
        void                   UpdateApp(double time) override;
    };
} // namespace Ignosi::BreakOut
