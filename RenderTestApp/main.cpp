#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"

#include <Window_GLFW.h>
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>

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

    auto                                 pVertexShader   = nate::Modules::Render::Shader::Create(vertex_shader_path);
    auto                                 pFragmentShader = nate::Modules::Render::Shader::Create(fragment_shader_path);
    nate::Modules::Render::ShaderProgram program(std::move(pFragmentShader), nullptr, std::move(pVertexShader));

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        0.5f,
        0.5f,
        0.0f, // top right
        0.5f,
        -0.5f,
        0.0f, // bottom right
        -0.5f,
        -0.5f,
        0.0f, // bottom left
        -0.5f,
        0.5f,
        0.0f // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0,
        1,
        3, // first Triangle
        1,
        2,
        3 // second Triangle
    };
    unsigned int VBO{0};
    unsigned int VAO{0};
    unsigned int EBO{0};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound
    // vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO;
    // keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
    // Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs)
    // when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!window.ShouldClose())
    {
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        program.Use();

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll
                                // do so to keep things a bit more organized
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window.GetGLFWWindow());
        window.PollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
