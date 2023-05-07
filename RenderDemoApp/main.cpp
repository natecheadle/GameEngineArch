#include "3D/Fly_Camera3D.h"
#include "3D/Object3D.h"
#include "IWindow.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"
#include "SquareMatrix4x4.hpp"
#include "Texture/Texture.h"

#include <Window_GLFW.h>
#include <glad/glad.h>

#include <GLFW/glfw3.h>

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

    nate::Modules::GUI::Window_GLFW window({SCR_WIDTH, SCR_HEIGHT}, "TEST_WINDOW");

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    auto pWallTex = std::make_shared<nate::Modules::Render::Texture>(
        wall_path,
        nate::Modules::Render::Texture::TextureUnit::Texture0);
    auto pFaceTex = std::make_shared<nate::Modules::Render::Texture>(
        awesomeface_path,
        nate::Modules::Render::Texture::TextureUnit::Texture1);

    auto pVertexShader   = nate::Modules::Render::Shader::Create(vertex_shader_path);
    auto pFragmentShader = nate::Modules::Render::Shader::Create(fragment_shader_path);
    auto pProgram =
        std::make_shared<nate::Modules::Render::ShaderProgram>(pFragmentShader.get(), nullptr, pVertexShader.get());

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
    nate::Modules::Render::Object3D     square(std::move(verts));
    square.Shader(std::move(pProgram));

    //  render loop
    //  -----------
    while (!window.ShouldClose())
    {
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera.Update(std::chrono::milliseconds(17));

        square.RotY(M_PI_4 / 100.0);

        square.Shader()->SetShaderVar("texture1", 0);
        square.Shader()->SetShaderVar("texture2", 1);
        square.Shader()->SetShaderVar("projection", camera.Projection());
        square.Shader()->SetShaderVar("model", square.ModelMatrix());
        square.Shader()->SetShaderVar("view", camera.View());

        square.Draw();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window.GetGLFWWindow());
        window.PollEvents();

        std::this_thread::sleep_for(std::chrono::milliseconds(17));
    }

    return 0;
}
