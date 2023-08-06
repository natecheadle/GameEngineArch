#include "PhysicsTestApp.h"

#include <Renderer/Renderer_OpenGL.h>
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
        std::filesystem::path sprite_dir = resource_dir / "Sprites";
    }
    void PhysicsTestApp::UpdateApp(double dt) {}
    void PhysicsTestApp::Shutdown() {}
} // namespace Ignosi::Examples::PhysicsTest