#include "3D/Object3D.h"
#include "IWindow.h"
#include "Renderer/Renderer.h"
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
    std::vector<nate::Modules::Render::VertexData> m_Verts;
    std::mutex                                     m_CallbackMutex;
    GUI::MouseClickedInfo                          m_LastMouseClick;
    GUI::CursorPosition                            m_LastPosition;
    GUI::WindowSize                                m_WindowSize;
    float                                          m_CamYaw{0.0};
    float                                          m_CamPitch{0.0};
    std::unique_ptr<Render::Object3D>              m_pCube;
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

        m_Verts = {
            {{{-0.5f, -0.5f, -0.5f}}, {}, {{0.0f, 0.0f}}},
            {{{0.5f, -0.5f, -0.5f}},  {}, {{1.0f, 0.0f}}},
            {{{0.5f, 0.5f, -0.5f}},   {}, {{1.0f, 1.0f}}},
            {{{0.5f, 0.5f, -0.5f}},   {}, {{1.0f, 1.0f}}},
            {{{-0.5f, 0.5f, -0.5f}},  {}, {{0.0f, 1.0f}}},
            {{{-0.5f, -0.5f, -0.5f}}, {}, {{0.0f, 0.0f}}},
            {{{-0.5f, -0.5f, 0.5f}},  {}, {{0.0f, 0.0f}}},
            {{{0.5f, -0.5f, 0.5f}},   {}, {{1.0f, 0.0f}}},
            {{{0.5f, 0.5f, 0.5f}},    {}, {{1.0f, 1.0f}}},
            {{{0.5f, 0.5f, 0.5f}},    {}, {{1.0f, 1.0f}}},
            {{{-0.5f, 0.5f, 0.5f}},   {}, {{0.0f, 1.0f}}},
            {{{-0.5f, -0.5f, 0.5f}},  {}, {{0.0f, 0.0f}}},
            {{{-0.5f, 0.5f, 0.5f}},   {}, {{1.0f, 0.0f}}},
            {{{-0.5f, 0.5f, -0.5f}},  {}, {{1.0f, 1.0f}}},
            {{{-0.5f, -0.5f, -0.5f}}, {}, {{0.0f, 1.0f}}},
            {{{-0.5f, -0.5f, -0.5f}}, {}, {{0.0f, 1.0f}}},
            {{{-0.5f, -0.5f, 0.5f}},  {}, {{0.0f, 0.0f}}},
            {{{-0.5f, 0.5f, 0.5f}},   {}, {{1.0f, 0.0f}}},
            {{{0.5f, 0.5f, 0.5f}},    {}, {{1.0f, 0.0f}}},
            {{{0.5f, 0.5f, -0.5f}},   {}, {{1.0f, 1.0f}}},
            {{{0.5f, -0.5f, -0.5f}},  {}, {{0.0f, 1.0f}}},
            {{{0.5f, -0.5f, -0.5f}},  {}, {{0.0f, 1.0f}}},
            {{{0.5f, -0.5f, 0.5f}},   {}, {{0.0f, 0.0f}}},
            {{{0.5f, 0.5f, 0.5f}},    {}, {{1.0f, 0.0f}}},
            {{{-0.5f, -0.5f, -0.5f}}, {}, {{0.0f, 1.0f}}},
            {{{0.5f, -0.5f, -0.5f}},  {}, {{1.0f, 1.0f}}},
            {{{0.5f, -0.5f, 0.5f}},   {}, {{1.0f, 0.0f}}},
            {{{0.5f, -0.5f, 0.5f}},   {}, {{1.0f, 0.0f}}},
            {{{-0.5f, -0.5f, 0.5f}},  {}, {{0.0f, 0.0f}}},
            {{{-0.5f, -0.5f, -0.5f}}, {}, {{0.0f, 1.0f}}},
            {{{-0.5f, 0.5f, -0.5f}},  {}, {{0.0f, 1.0f}}},
            {{{0.5f, 0.5f, -0.5f}},   {}, {{1.0f, 1.0f}}},
            {{{0.5f, 0.5f, 0.5f}},    {}, {{1.0f, 0.0f}}},
            {{{0.5f, 0.5f, 0.5f}},    {}, {{1.0f, 0.0f}}},
            {{{-0.5f, 0.5f, 0.5f}},   {}, {{0.0f, 0.0f}}},
            {{{-0.5f, 0.5f, -0.5f}},  {}, {{0.0f, 1.0f}}}
        };
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

        m_pCube = std::make_unique<Render::Object3D>(GetRenderer(), m_Verts);
        m_pCube->Shader(std::move(pProgram));
        m_pCube->Textures({std::move(pWallTex), std::move(pFaceTex)});
        m_pCamera = std::make_unique<Render::Fly_Camera3D>(GetWindow());
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
        auto* pRenderer = GetRenderer();

        pRenderer->SetShaderVar(m_pCube->Shader().get(), "texture1", 0);
        pRenderer->SetShaderVar(m_pCube->Shader().get(), "texture2", 1);
        pRenderer->SetShaderVar(m_pCube->Shader().get(), "projection", m_pCamera->Projection());
        pRenderer->SetShaderVar(m_pCube->Shader().get(), "model", m_pCube->ModelMatrix());
        pRenderer->SetShaderVar(m_pCube->Shader().get(), "view", m_pCamera->View());

        pRenderer->Draw(m_pCube.get());
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
