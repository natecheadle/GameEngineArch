#include "3D/Fly_Camera3D.h"
#include "3D/Object3D.h"
#include "IWindow.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"
#include "SquareMatrix4x4.hpp"

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

    nate::Modules::GUI::Window_GLFW window({SCR_WIDTH, SCR_HEIGHT}, "TEST_WINDOW");

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    auto pVertexShader   = nate::Modules::Render::Shader::Create(vertex_shader_path);
    auto pFragmentShader = nate::Modules::Render::Shader::Create(fragment_shader_path);
    auto pProgram        = std::make_shared<nate::Modules::Render::ShaderProgram>(
        std::move(pFragmentShader),
        nullptr,
        std::move(pVertexShader));

    std::vector<nate::Modules::Render::VertexData> verts{
        {{{0.5f, 0.5f, 0.0f}},   {}, {}},
        {{{0.5f, -0.5f, 0.0f}},  {}, {}},
        {{{-0.5f, -0.5f, 0.0f}}, {}, {}},
        {{{-0.5f, 0.5f, 0.0f}},  {}, {}},
    };

    std::vector<std::uint32_t> indexes{
        // note that we start from 0!
        0,
        1,
        3, // first Triangle
        1,
        2,
        3 // second Triangle
    };

    nate::Modules::Render::Fly_Camera3D camera(static_cast<nate::Modules::GUI::IWindow*>(&window));
    nate::Modules::Render::Object3D     square(std::move(verts), std::move(indexes));
    square.Shader(std::move(pProgram));

    // render loop
    // -----------
    while (!window.ShouldClose())
    {
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        camera.Update(std::chrono::milliseconds(17));

        square.Shader()->SetShaderVar(
            "model",
            nate::Modules::SquareMatrix4x4<float>::identity<nate::Modules::SquareMatrix4x4<float>>());
        square.Shader()->SetShaderVar("view", camera.View());
        square.Shader()->SetShaderVar("projection", camera.Projection());

        square.Draw();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window.GetGLFWWindow());
        window.PollEvents();

        std::this_thread::sleep_for(std::chrono::milliseconds(17));
    }

    return 0;
}
