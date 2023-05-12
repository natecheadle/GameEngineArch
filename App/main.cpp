#include "3D/Object3D.h"
#include "IWindow.h"
#include "Renderer/Renderer.h"
#include "Vector3.hpp"
#include "WindowMessages.hpp"
#include "WindowSize.hpp"

#include <3D/Fly_Camera3D.h>
#include <App.h>
#include <DebugCast.hpp>
#include <Messages/MouseClicked.hpp>
#include <Messages/WindowResized.hpp>
#include <Shader/Shader.h>

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

class TestApp : public App::App
{
    std::mutex                                     m_CallbackMutex;
    GUI::MouseClickedInfo                          m_LastMouseClick;
    GUI::CursorPosition                            m_LastPosition;
    GUI::WindowSize                                m_WindowSize;
    float                                          m_CamYaw{0.0};
    float                                          m_CamPitch{0.0};
    std::vector<std::unique_ptr<Render::Object3D>> m_Cubes;
    std::unique_ptr<Render::Fly_Camera3D>          m_pCamera;

  public:
    TestApp(std::unique_ptr<Render::Renderer> pRenderer, const GUI::WindowSize& window_size, std::string window_name)
        : App(std::move(pRenderer), window_size, std::move(window_name))
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
        std::filesystem::path shader_dir(std::string(SHADER_DIR));
        shader_dir /= "Shaders";

        auto vertex_shader_path   = shader_dir / "vertex_shader.vert";
        auto fragment_shader_path = shader_dir / "fragment_shader.frag";
        auto awesomeface_path     = shader_dir / "awesomeface.png";
        auto wall_path            = shader_dir / "wall.jpg";

        auto pWallTex = GetRenderer()->CreateTexture(wall_path, nate::Modules::Render::TextureUnit::Texture0);
        auto pFaceTex = GetRenderer()->CreateTexture(awesomeface_path, nate::Modules::Render::TextureUnit::Texture1);

        auto pVertexShader   = GetRenderer()->CreateShader(vertex_shader_path);
        auto pFragmentShader = GetRenderer()->CreateShader(fragment_shader_path);
        auto pProgram        = GetRenderer()->CreateShaderProgram(pFragmentShader.get(), nullptr, pVertexShader.get());

        const size_t numOfCubes{10};
        m_Cubes.resize(numOfCubes);

        m_Cubes[0] = Render::Object3D::CreateCube(GetRenderer());
        m_Cubes[0]->Shader(std::move(pProgram));
        m_Cubes[0]->Textures({std::move(pWallTex), std::move(pFaceTex)});

        Vector3<float> cubePositions[] = {
            Vector3<float>(0.0f, 0.0f, 0.0f),
            Vector3<float>(2.0f, 5.0f, -15.0f),
            Vector3<float>(-1.5f, -2.2f, -2.5f),
            Vector3<float>(-3.8f, -2.0f, -12.3f),
            Vector3<float>(2.4f, -0.4f, -3.5f),
            Vector3<float>(-1.7f, 3.0f, -7.5f),
            Vector3<float>(1.3f, -2.0f, -2.5f),
            Vector3<float>(1.5f, 2.0f, -2.5f),
            Vector3<float>(1.5f, 0.2f, -1.5f),
            Vector3<float>(-1.3f, 1.0f, -1.5f)};

        static_assert(sizeof(cubePositions) / sizeof(Vector3<float>) == numOfCubes, "Incorrect number of cubes");

        for (size_t i = 1; i < numOfCubes; ++i)
        {
            m_Cubes[i] = std::make_unique<Render::Object3D>(*m_Cubes[0]);
            m_Cubes[i]->Origin(cubePositions[i]);
        }

        m_pCamera = std::make_unique<Render::Fly_Camera3D>(GetWindow());
    }

    void Shutdown() override
    {
        m_pCamera.reset();
        m_Cubes.clear();
    }

    void UpdateApp(std::chrono::nanoseconds time) override
    {
        // TODO this should be handled automatically
        m_pCamera->Update(time);
        auto* pRenderer = GetRenderer();

        for (const auto& cube : m_Cubes)
        {
            pRenderer->SetShaderVar(cube->Shader().get(), "texture1", 0);
            pRenderer->SetShaderVar(cube->Shader().get(), "texture2", 1);
            pRenderer->SetShaderVar(cube->Shader().get(), "projection", m_pCamera->Projection());
            pRenderer->SetShaderVar(cube->Shader().get(), "model", cube->ModelMatrix());
            pRenderer->SetShaderVar(cube->Shader().get(), "view", m_pCamera->View());
            pRenderer->Draw(cube.get());
        }
    }
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
