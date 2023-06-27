#include "PhysicsTestApp.h"
#include "RigidBody2D.h"

#include <iostream>

int main()
{
    try
    {
        PhysicsTestApp app(std::make_unique<ECS::World<Render::Mesh3D, Render::Sprite, Physics::RigidBody2D>>());
        int            code = app.Run();
        app.Close();
        return code;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }

    return 1;
}
