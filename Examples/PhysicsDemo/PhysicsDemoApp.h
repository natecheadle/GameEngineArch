#pragma once

#include <3D/Mesh3D.h>
#include <3D/Sprite.h>
#include <App.h>
#include <IWindow.h>
#include <IWorld.h>
#include <KinematicData.h>
#include <PhysicsSystem.h>
#include <Renderer/Renderer.h>
#include <RigidBody2D.h>
#include <WindowSize.hpp>
#include <World.h>

#include <memory>
#include <string>

namespace Ignosi::Examples::PhysicsDemo
{
    using PhysicsWorld = Modules::ECS::
        World<Modules::Render::Mesh3D, Modules::Render::Sprite, Modules::Physics::RigidBody2D, Modules::Physics::KinematicData>;

    class PhysicsDemoApp : public Modules::App::App
    {
        PhysicsWorld* m_pWorld;

        Modules::Render::Renderer*       m_pRenderer;
        Modules::Physics::PhysicsSystem* m_pPhysicsSystem;
        Modules::GUI::IWindow*           m_pWindow;

      public:
        PhysicsDemoApp(std::unique_ptr<Modules::ECS::IWorld> pWorld, const Modules::GUI::WindowSize& window_size, std::string window_name);
        ~PhysicsDemoApp() override = default;

      protected:
        void                   UpdateApp(double dt) override;
        Modules::GUI::IWindow* Initialize() override;
        void                   Shutdown() override;
    };
} // namespace Ignosi::Examples::PhysicsDemo
