#include <Renderer.h>
#include <Window_GLFW.h>

#include <cassert>
#include <chrono>
#include <thread>

using namespace nate::Modules;
int main()
{
    GUI::Window_GLFW window({800, 600}, "Test Window");
    assert(window.IsValid());

    Render::Renderer renderer;
    renderer.Initialize(&window);
    while (renderer.IsRunning())
    {
        Render::Renderer::RenderFrame();
    }

    return 0;
}