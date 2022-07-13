#include <Renderer.h>
#include <Window_GLFW.h>

#include <cassert>
#include <chrono>
#include <thread>

bool ShouldClose = false;
void OnShouldClose()
{
    ShouldClose = true;
}

using namespace nate::Modules;
int main()
{
    GUI::Window_GLFW window({800, 600}, "Test Window");
    assert(window.IsValid());
    window.AttachOnCloseCallback(&OnShouldClose);

    Render::Renderer renderer;
    renderer.Initialize(&window);
    while (renderer.IsRunning())
    {
        window.PollEvents();

        if (window.ShouldClose())
            renderer.Shutdown();

        Render::Renderer::RenderFrame();
    }
    window.Close();

    return 0;
}
