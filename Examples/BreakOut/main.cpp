#include <3D/Camera2D.h>
#include <3D/Fly_Camera.h>
#include <3D/Light_Directional.h>
#include <3D/Light_Point.h>
#include <3D/Light_Spotlight.h>
#include <3D/Material.h>
#include <3D/Mesh3D.h>
#include <3D/Model3D.h>
#include <App.h>
#include <DebugCast.hpp>
#include <IWindow.h>
#include <Messages/MouseClicked.hpp>
#include <Messages/WindowResized.hpp>
#include <Renderer/Renderer.h>
#include <Shader/Shader.h>
#include <SquareMatrix4x4.hpp>
#include <Vector3.hpp>
#include <WindowMessages.hpp>
#include <WindowSize.hpp>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

using namespace nate::Modules;
using namespace std::chrono_literals;

class TestApp : public App::App
{

  public:
    TestApp(std::unique_ptr<Render::Renderer> pRenderer, const GUI::WindowSize& window_size, std::string window_name)
        : App(std::move(pRenderer), window_size, std::move(window_name))
    {
    }

  protected:
    void Initialize() override {}

    void Shutdown() override {}

    void UpdateApp(std::chrono::nanoseconds time) override {}
};

int main()
{
    try
    {
        std::unique_ptr<Render::Renderer> pRenderer = Render::Renderer::Create();
        TestApp                           app(std::move(pRenderer), {800, 600}, "Test Window");
        int                               code = app.Run();
        app.Close();
        return code;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }

    return 1;
}
