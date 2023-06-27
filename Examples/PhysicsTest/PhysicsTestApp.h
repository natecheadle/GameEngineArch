#include "WindowSize.hpp"

#include <3D/Mesh3D.h>
#include <3D/Sprite.h>
#include <App.h>
#include <RigidBody2D.h>
#include <World.h>

#include <string_view>

using namespace nate::Modules;

class PhysicsTestApp : public App::App
{
    static constexpr GUI::WindowSize  WINDOW_SIZE{800, 600};
    static constexpr std::string_view WINDOW_NAME{"Physics Test App"};

    std::unique_ptr<ECS::World<Render::Mesh3D, Render::Sprite, Physics::RigidBody2D>> m_pWorld;

  public:
    PhysicsTestApp(std::unique_ptr<ECS::World<Render::Mesh3D, Render::Sprite, Physics::RigidBody2D>> pWorld);

  protected:
    void Initialize() override;
    void UpdateApp(double dt) override;
    void Shutdown() override;
};
