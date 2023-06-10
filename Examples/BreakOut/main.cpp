#include "BreakOutApp.h"
#include "Physics/PhysicsSystem.h"

using namespace nate::Modules;

int main()
{
    try
    {
        nate::BreakOut::BreakOutApp app(
            std::make_unique<ECS::World<Render::Mesh3D, Render::Sprite, Physics::RigidBody2D>>(),
            {800, 600},
            "Test Window");
        int code = app.Run();
        app.Close();
        return code;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }

    return 1;
}
