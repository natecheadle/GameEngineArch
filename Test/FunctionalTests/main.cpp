#include "WindowMessages.hpp"

#include <DebugCast.hpp>
#include <Messages/WindowResized.hpp>
#include <Renderer.h>
#include <Window_GLFW.h>

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

using namespace nate::Modules;

bool ShouldClose = false;
void OnShouldClose(const Messaging::Message<GUI::WindowMessages>* pMessage)
{
    assert(pMessage->ID() == GUI::WindowMessages::WindowClosed);
    ShouldClose = true;
}

void OnWindowResize(const Messaging::Message<GUI::WindowMessages>* pMessage)
{
    assert(pMessage->ID() == GUI::WindowMessages::WindowResized);
    const auto* pResized = DebugCast<const GUI::WindowResized*>(pMessage);
    std::cout << "Window Resized...New Width = " << pResized->GetData().Width()
              << " New Height = " << pResized->GetData().Height() << "\r\n";
}

int main()
{
    GUI::Window_GLFW window({800, 600}, "Test Window");
    assert(window.IsValid());
    window.SubscribeToMessage(&window, GUI::WindowMessages::WindowClosed, &OnShouldClose);
    window.SubscribeToMessage(&window, GUI::WindowMessages::WindowResized, &OnWindowResize);

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
