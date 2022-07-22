#include "WindowMessages.hpp"

#include <DebugCast.hpp>
#include <Messages/WindowResized.hpp>
#include <Renderer.h>
#include <Shader.h>
#include <Window_GLFW.h>

#include <cassert>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>

using namespace nate::Modules;

void OnWindowResize(const Messaging::Message<GUI::WindowMessages>* pMessage)
{
    assert(pMessage->ID() == GUI::WindowMessages::WindowResized);
    const auto* pResized = DebugCast<const GUI::WindowResized*>(pMessage);
    std::cout << "Window Resized...New Width = " << pResized->GetData()->Width()
              << " New Height = " << pResized->GetData()->Height() << "\r\n";
}

int main()
{
    try
    {
        GUI::Window_GLFW window({800, 600}, "Test Window");
        assert(window.IsValid());
        window.SubscribeToMessage(&window, GUI::WindowMessages::WindowResized, &OnWindowResize);

        std::filesystem::path shader_dir(APP_OUT_DIR);
        shader_dir /= "Shaders";

        Render::Renderer renderer;
        renderer.Initialize(&window, std::move(shader_dir));

        while (!renderer.IsInitialized() && renderer.IsRunning())
        {
            std::this_thread::yield();
            Render::Renderer::RenderFrame();
        }

        while (renderer.IsRunning())
        {
            window.PollEvents();

            if (window.ShouldClose())
                renderer.Shutdown();

            Render::Renderer::RenderFrame();
        }
        window.Close();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
        return 1;
    }
    return 0;
}
