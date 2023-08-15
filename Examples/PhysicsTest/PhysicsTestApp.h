#pragma once

#include "Ball.h"
#include "Wall.h"
#include "WindowSize.hpp"

#include <3D/Camera.h>
#include <3D/Mesh3D.h>
#include <3D/Sprite.h>
#include <App.h>
#include <RigidBody2D.h>
#include <World.h>

#include <memory>
#include <string_view>

using namespace Ignosi::Modules;
namespace Ignosi::Examples::PhysicsTest
{
    class PhysicsTestApp : public App::App
    {
        static constexpr GUI::WindowSize  WINDOW_SIZE{800, 600};
        static constexpr std::string_view WINDOW_NAME{"Physics Test App"};

        std::unique_ptr<ECS::World<Render::Mesh3D, Render::Sprite, Physics::RigidBody2D>> m_pWorld;
        std::unique_ptr<Modules::Render::Camera>                                          m_pCamera;
        Modules::Render::ShaderProgram_ptr                                                m_pShader;
        std::unique_ptr<Ball>                                                             m_pBall;
        std::unique_ptr<Wall>                                                             m_pTopWall;
        std::unique_ptr<Wall>                                                             m_pBottomWall;
        std::unique_ptr<Wall>                                                             m_pLeftWall;
        std::unique_ptr<Wall>                                                             m_pRightWall;

      public:
        PhysicsTestApp(std::unique_ptr<ECS::World<Render::Mesh3D, Render::Sprite, Physics::RigidBody2D>> pWorld);

      protected:
        void Initialize() override;
        void UpdateApp(double dt) override;
        void Shutdown() override;
    };
} // namespace Ignosi::Examples::PhysicsTest
