#include "PhysicsDemoApp.h"

#include <3D/Camera.h>
#include <3D/Mesh3D.h>
#include <3D/Sprite.h>
#include <App.h>
#include <IWindow.h>
#include <IWorld.h>
#include <KinematicData.h>
#include <Renderer/Renderer_OpenGL.h>
#include <RigidBody2D.h>
#include <WindowSize.hpp>

#include <memory>
#include <string>
#include <utility>

namespace Ignosi::Examples::PhysicsDemo
{

    PhysicsDemoApp::PhysicsDemoApp(
        std::unique_ptr<Modules::ECS::IWorld> pWorld,
        const Modules::GUI::WindowSize&       window_size,
        std::string                           window_name)
        : Modules::App::App(std::move(pWorld))
    {
        m_pWorld    = dynamic_cast<PhysicsWorld*>(App::GetWorld());
        m_pRenderer = m_pWorld->CreateSystem<Modules::Render::Renderer_OpenGL, Modules::Render::Mesh3D, Modules::Render::Sprite>();
        m_pPhysicsSystem =
            m_pWorld->CreateSystem<Modules::Physics::PhysicsSystem, Modules::Physics::RigidBody2D, Modules::Physics::KinematicData>();
        m_pWindow = m_pRenderer->InitializeWindow(window_size, std::move(window_name));
    }

    void PhysicsDemoApp::UpdateApp(double /*dt*/) {}

    Modules::GUI::IWindow* PhysicsDemoApp::Initialize()
    {
        auto pCamera = std::make_unique<Modules::Render::Camera>(m_pWindow);
        pCamera->Near(-1.0);
        pCamera->Far(1.0);
        m_pRenderer->AttachedCamera(std::move(pCamera));

        return m_pWindow;
    }

    void PhysicsDemoApp::Shutdown() {}
} // namespace Ignosi::Examples::PhysicsDemo
