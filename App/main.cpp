#include "IWindow.h"
#include "WindowMessages.hpp"
#include "WindowSize.hpp"

#include <App.h>
#include <DebugCast.hpp>
#include <Messages/WindowResized.hpp>
#include <Renderer.h>
#include <Shader.h>
#include <Window_GLFW.h>

#include <cassert>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <thread>

nate::Modules::Render::VertexPoint3D cube_vertices[] = {
    {{-1.0f, 1.0f, 1.0f},   0xff000000},
    {{1.0f, 1.0f, 1.0f},    0xff0000ff},
    {{-1.0f, -1.0f, 1.0f},  0xff00ff00},
    {{1.0f, -1.0f, 1.0f},   0xff00ffff},
    {{-1.0f, 1.0f, -1.0f},  0xffff0000},
    {{1.0f, 1.0f, -1.0f},   0xffff00ff},
    {{-1.0f, -1.0f, -1.0f}, 0xffffff00},
    {{1.0f, -1.0f, -1.0f},  0xffffffff},
};

const uint16_t cube_tri_list[] = {
    0, 1, 2, 1, 3, 2, 4, 6, 5, 5, 6, 7, 0, 2, 4, 4, 2, 6, 1, 5, 3, 5, 7, 3, 0, 4, 1, 4, 5, 1, 2, 3, 6, 6, 3, 7,
};

using namespace nate::Modules;

void OnWindowResize(const Messaging::Message<GUI::WindowMessages>* pMessage)
{
    assert(pMessage->ID() == GUI::WindowMessages::WindowResized);
    const auto* pResized = DebugCast<const GUI::WindowResized*>(pMessage);
    std::cout << "Window Resized...New Width = " << pResized->GetData()->Width()
              << " New Height = " << pResized->GetData()->Height() << "\r\n";
}

class TestApp : public App::App {
  public:
    TestApp(std::unique_ptr<GUI::IWindow> pWindow, std::unique_ptr<Render::IRenderer> pRenderer)
        : App(std::move(pWindow), std::move(pRenderer))
    {
    }

  protected:
    void UpdateApp() override {}
};

int main()
{
    try
    {
        std::unique_ptr<GUI::IWindow> pWindow =
            std::make_unique<GUI::Window_GLFW>(GUI::WindowSize(800, 600), "Test Window");
        assert(pWindow->IsValid());
        pWindow->SubscribeToMessage(pWindow.get(), GUI::WindowMessages::WindowResized, &OnWindowResize);

        std::filesystem::path shader_dir(APP_OUT_DIR);
        shader_dir /= "Shaders";

        std::vector<Render::VertexPoint3D> points(cube_vertices, cube_vertices + 8);
        std::vector<std::uint16_t>         indices(cube_tri_list, cube_tri_list + 36);
        Render::Object3D                   cube(std::move(points), std::move(indices));
        std::unique_ptr<Render::IRenderer> pRenderer = std::make_unique<Render::Renderer>();
        pRenderer->RenderObject(&cube);
        pRenderer->Initialize(pWindow.get(), std::move(shader_dir));

        TestApp app(std::move(pWindow), std::move(pRenderer));
        return app.Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }

    return 1;
}
