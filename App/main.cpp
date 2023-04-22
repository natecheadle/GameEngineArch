#include "IWindow.h"
#include "Shader/BGFX_Shader.h"
#include "WindowMessages.hpp"
#include "WindowSize.hpp"

#include <3D/Camera/Fly_Camera3D.h>
#include <App.h>
#include <DebugCast.hpp>
#include <Messages/MouseClicked.hpp>
#include <Messages/WindowResized.hpp>
#include <Renderer.h>
#include <Shader/Shader.h>
#include <Window_GLFW.h>

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
}

class TestApp : public App::App
{
    std::mutex                            m_CallbackMutex;
    GUI::MouseClickedInfo                 m_LastMouseClick;
    GUI::CursorPosition                   m_LastPosition;
    GUI::WindowSize                       m_WindowSize;
    float                                 m_CamYaw{0.0};
    float                                 m_CamPitch{0.0};
    std::shared_ptr<Render::Object3D>     m_pCube;
    std::shared_ptr<Render::Fly_Camera3D> m_pCamera;

  public:
    TestApp(std::unique_ptr<GUI::IWindow> pWindow, std::unique_ptr<Render::Renderer> pRenderer)
        : App(std::move(pWindow), std::move(pRenderer))
    {

        GetWindow()->SubscribeToMessage(
            this,
            GUI::WindowMessages::MouseClicked,
            [this](const GUI::WindowMessage* pMessage) {
                std::unique_lock<std::mutex> lock(m_CallbackMutex);

                m_LastMouseClick = *(DebugCast<const GUI::MouseClicked*>(pMessage)->GetData());
            });
        GetWindow()->SubscribeToMessage(
            this,
            GUI::WindowMessages::WindowResized,
            [this](const GUI::WindowMessage* pMessage) {
                std::unique_lock<std::mutex> lock(m_CallbackMutex);

                m_WindowSize = *(DebugCast<const GUI::WindowResized*>(pMessage)->GetData());
            });
    }

  protected:
    void Initialize() override
    {
        std::filesystem::path shader_dir(APP_OUT_DIR);
        shader_dir /= "Shaders";

        std::unique_ptr<Render::Shader> fragmentShader{
            std::make_unique<Render::BGFX_Shader>("fs_cubes.sc.bin", shader_dir, GetRenderer())};
        std::unique_ptr<Render::Shader> vertexShader{
            std::make_unique<Render::BGFX_Shader>("vs_cubes.sc.bin", shader_dir, GetRenderer())};

        m_pCube = std::make_shared<Render::Object3D>(
            std::vector<Render::VertexPoint3D>(cube_vertices, cube_vertices + 8),
            std::vector<std::uint16_t>(cube_tri_list, cube_tri_list + 36),
            std::make_shared<Render::Material>(std::move(fragmentShader), std::move(vertexShader), GetRenderer()));
        m_pCamera = std::make_shared<Render::Fly_Camera3D>(GetWindow());
        m_pCamera->Translate({0, 0, -5});
    }

    void Shutdown() override
    {
        m_pCamera.reset();
        m_pCube.reset();
    }

    void UpdateApp(std::chrono::nanoseconds time) override
    {
        // TODO this should be handled automatically
        m_pCamera->Update(time);
        GetRenderer()->AttachCamera(m_pCamera);
        GetRenderer()->RenderObject(m_pCube);
    }
};

int main()
{
    try
    {
        std::unique_ptr<GUI::IWindow> pWindow =
            std::make_unique<GUI::Window_GLFW>(GUI::WindowSize(800, 600), "Test Window");
        assert(pWindow->IsValid());
        pWindow->SubscribeToMessage(pWindow.get(), GUI::WindowMessages::WindowResized, &OnWindowResize);

        std::unique_ptr<Render::Renderer> pRenderer = std::make_unique<Render::Renderer>();
        pRenderer->Initialize(pWindow.get());

        TestApp app(std::move(pWindow), std::move(pRenderer));
        int     code = app.Run();
        app.Close();
        return code;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what();
    }

    return 1;
}
