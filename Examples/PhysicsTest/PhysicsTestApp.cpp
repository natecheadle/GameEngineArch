#include "PhysicsTestApp.h"

#include <Renderer/Renderer_OpenGL.h>

PhysicsTestApp::PhysicsTestApp(std::unique_ptr<ECS::World<Render::Mesh3D, Render::Sprite, Physics::RigidBody2D>> pWorld)
    : App::App(
          pWorld->CreateSystem<Render::Renderer_OpenGL, Render::Mesh3D, Render::Sprite>(),
          WINDOW_SIZE,
          std::string(WINDOW_NAME))
    , m_pWorld(std::move(pWorld))
{
}

void PhysicsTestApp::Initialize() {}
void PhysicsTestApp::UpdateApp(double dt) {}
void PhysicsTestApp::Shutdown() {}