#include "PhysicsDemoApp.h"

#include <exception>
#include <memory>

using namespace Ignosi;

int main()
{
    try
    {
        Ignosi::Examples::PhysicsDemo::PhysicsDemoApp app(
            std::make_unique<Examples::PhysicsDemo::PhysicsWorld>(),
            {800, 600},
            "Test Window");
        const int code = app.Run();
        app.Close();
        return code;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }

    return 1;
}
