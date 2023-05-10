#include "3D/Fly_Camera3D.h"
#include "IWindow.h"
#include "Renderer/Renderer_OpenGL.h"
#include "SquareMatrix4x4.hpp"

#include <Window_GLFW.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

// settings
const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    std::filesystem::path shader_dir(std::string(SHADER_DIR));

    auto vertex_shader_path   = shader_dir / "vertex_shader.vert";
    auto fragment_shader_path = shader_dir / "fragment_shader.frag";
    auto awesomeface_path     = shader_dir / "awesomeface.png";
    auto wall_path            = shader_dir / "wall.jpg";

    nate::Modules::Render::Renderer::SetInstance(std::make_unique<nate::Modules::Render::Renderer_OpenGL>());
    auto* pRenderer = nate::Modules::Render::Renderer::GetInstance();

    nate::Modules::GUI::Window_GLFW window({SCR_WIDTH, SCR_HEIGHT}, "TEST_WINDOW");

    auto pWallTex = pRenderer->CreateTexture(wall_path, nate::Modules::Render::TextureUnit::Texture0);
    auto pFaceTex = pRenderer->CreateTexture(awesomeface_path, nate::Modules::Render::TextureUnit::Texture1);

    auto pVertexShader   = pRenderer->CreateShader(vertex_shader_path);
    auto pFragmentShader = pRenderer->CreateShader(fragment_shader_path);
    auto pProgram        = pRenderer->CreateShaderProgram(pFragmentShader.get(), nullptr, pVertexShader.get());

    std::vector<nate::Modules::Render::VertexData> verts{
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

    nate::Modules::Render::Fly_Camera3D camera(static_cast<nate::Modules::GUI::IWindow*>(&window));
    auto                                pSquare = pRenderer->CreateObject(std::move(verts));
    pSquare->Shader(std::move(pProgram));
    pSquare->Textures({pWallTex, pFaceTex});

    //  render loop
    //  -----------
    while (!window.ShouldClose())
    {

        pRenderer->ClearColorBuffer();
        pRenderer->ClearDepthBuffer();

        camera.Update(std::chrono::milliseconds(17));

        pSquare->RotY(M_PI_4 / 100.0);

        pRenderer->SetShaderVar(pSquare->Shader().get(), "texture1", 0);
        pRenderer->SetShaderVar(pSquare->Shader().get(), "texture2", 1);
        pRenderer->SetShaderVar(pSquare->Shader().get(), "projection", camera.Projection());
        pRenderer->SetShaderVar(pSquare->Shader().get(), "model", pSquare->ModelMatrix());
        pRenderer->SetShaderVar(pSquare->Shader().get(), "view", camera.View());

        pRenderer->Draw(pSquare.get());

        pRenderer->SwapBuffers(&window);
        window.PollEvents();

        std::this_thread::sleep_for(std::chrono::milliseconds(17));
    }

    return 0;
}
