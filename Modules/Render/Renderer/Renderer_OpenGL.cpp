#include "Renderer_OpenGL.h"

#include "../Shader/OpenGL_Shader.h"
#include "../Shader/OpenGL_ShaderProgram.h"
#include "../Texture/OpenGL_Texture.h"
#include "DebugCast.hpp"
#include "Renderer/VertexBuffer.h"
#include "VertexBuffer_OpenGL.h"
#include "Window_GLFW.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace nate::Modules::Render
{
    Renderer_OpenGL::~Renderer_OpenGL()
    {
        m_pWin.reset();
    }

    GUI::IWindow* Renderer_OpenGL::Initialize(const GUI::WindowSize& size, std::string name)
    {
#ifdef __APPLE__
        // Mac requires the window to be in the main thread.
        m_pWin = std::make_unique<GUI::Window_GLFW>(size, std::move(name));

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
        {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        ExecuteFunction([&]() -> void {
            glfwMakeContextCurrent(m_pWin->GetGLFWWindow());
            // configure global opengl state
            // -----------------------------
            // TODO this should be user configurable.
            glEnable(GL_DEPTH_TEST);
        });

#else

        ExecuteFunction([&]() -> void {
            // Windows requires the windoww and conntext to be in the same thread, but the window can be on any thread.
            m_pWin = std::make_unique<GUI::Window_GLFW>(size, std::move(name));

            // glad: load all OpenGL function pointers
            // ---------------------------------------
            if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
            {
                throw std::runtime_error("Failed to initialize GLAD");
            }
            // configure global opengl state
            // -----------------------------
            // TODO this should be user configurable.
            glEnable(GL_DEPTH_TEST);
        });
#endif
        return m_pWin.get();
    }

    VertexBuffer_ptr Renderer_OpenGL::CreateBuffer(const std::vector<VertexData>& vertexes)
    {
        VertexBuffer_ptr rslt;
        ExecuteFunction([&]() -> void {
            rslt = VertexBuffer_ptr(new VertexBuffer_OpenGL(vertexes), [this](VertexBuffer* pObj) { Destroy(pObj); });
        });
        return rslt;
    }

    VertexBuffer_ptr Renderer_OpenGL::CreateBuffer(
        const std::vector<VertexData>&    vertexes,
        const std::vector<std::uint32_t>& indeces)
    {
        VertexBuffer_ptr rslt;
        ExecuteFunction([&]() -> void {
            rslt = VertexBuffer_ptr(new VertexBuffer_OpenGL(vertexes, indeces), [this](VertexBuffer* pObj) {
                Destroy(pObj);
            });
        });
        return rslt;
    }

    Shader_ptr Renderer_OpenGL::CreateShader(const std::filesystem::path& path)
    {
        Shader_ptr rslt;
        ExecuteFunction([&]() -> void {
            rslt = Shader_ptr(OpenGL_Shader::Create(path).release(), [this](Shader* pShader) { Destroy(pShader); });
        });
        return rslt;
    }

    Shader_ptr Renderer_OpenGL::CreateShader(const std::filesystem::path& path, ShaderType type)
    {
        Shader_ptr rslt;
        ExecuteFunction([&]() -> void {
            rslt =
                Shader_ptr(OpenGL_Shader::Create(path, type).release(), [this](Shader* pShader) { Destroy(pShader); });
        });
        return rslt;
    }

    ShaderProgram_ptr Renderer_OpenGL::CreateShaderProgram(
        const Shader* pFragmentShader,
        const Shader* pGeometryShader,
        const Shader* pVertexShader)
    {
        ShaderProgram_ptr rslt;
        ExecuteFunction([&]() -> void {
            rslt = ShaderProgram_ptr(
                new OpenGL_ShaderProgram(pFragmentShader, pGeometryShader, pVertexShader),
                [this](ShaderProgram* pProgram) { Destroy(pProgram); });
        });
        return rslt;
    }

    Texture_ptr Renderer_OpenGL::CreateTexture(const std::filesystem::path& path, TextureUnit unit)
    {
        Texture_ptr rslt;
        ExecuteFunction([&]() -> void {
            rslt = Texture_ptr(new OpenGL_Texture(path, unit), [this](Texture* pTex) { Destroy(pTex); });
        });
        return rslt;
    }

    Texture_ptr Renderer_OpenGL::CreateTexture(const ImageFile& image, TextureUnit unit)
    {
        Texture_ptr rslt;
        ExecuteFunction([&]() -> void {
            rslt = Texture_ptr(new OpenGL_Texture(image, unit), [this](Texture* pTex) { Destroy(pTex); });
        });
        return rslt;
    }

    void Renderer_OpenGL::Draw(Object3D* pObj)
    {
        assert(pObj);
        ExecuteFunction([pObj]() -> void { pObj->Draw(); });
    }

    void Renderer_OpenGL::SetShaderVar(ShaderProgram* pShader, const std::string& name, bool value)
    {
        assert(pShader);
        ExecuteFunction([&]() -> void { pShader->SetShaderVar(name, value); });
    }

    void Renderer_OpenGL::SetShaderVar(ShaderProgram* pShader, const std::string& name, int value)
    {
        assert(pShader);
        ExecuteFunction([&]() -> void { pShader->SetShaderVar(name, value); });
    }

    void Renderer_OpenGL::SetShaderVar(ShaderProgram* pShader, const std::string& name, float value)
    {
        assert(pShader);
        ExecuteFunction([&]() -> void { pShader->SetShaderVar(name, value); });
    }

    void Renderer_OpenGL::SetShaderVar(
        ShaderProgram*                pShader,
        const std::string&            name,
        const SquareMatrix<4, float>& value)
    {
        assert(pShader);
        ExecuteFunction([&]() -> void { pShader->SetShaderVar(name, value); });
    }

    void Renderer_OpenGL::SetShaderVar(ShaderProgram* pShader, const std::string& name, const Vector<3, float>& value)
    {
        assert(pShader);
        ExecuteFunction([&]() -> void { pShader->SetShaderVar(name, value); });
    }

    void Renderer_OpenGL::SetShaderVar(ShaderProgram* pShader, const std::string& name, const Vector<4, float>& value)
    {
        assert(pShader);
        ExecuteFunction([&]() -> void { pShader->SetShaderVar(name, value); });
    }

    void Renderer_OpenGL::ClearDepthBuffer()
    {
        ExecuteFunction([]() -> void { glClear(GL_DEPTH_BUFFER_BIT); });
    }

    void Renderer_OpenGL::ClearColorBuffer()
    {
        ExecuteFunction([]() -> void {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        });
    }

    void Renderer_OpenGL::SwapBuffers()
    {
#ifdef __APPLE__
        glfwSwapBuffers(this->m_pWin->GetGLFWWindow());
        m_pWin->PollEvents();
#else
        ExecuteFunction([this]() -> void {
            glfwSwapBuffers(this->m_pWin->GetGLFWWindow());
            m_pWin->PollEvents();
        });
#endif
    }

    bool Renderer_OpenGL::Validate(void* pVoid)
    {
        if (!pVoid)
        {
            std::cerr << "[ERROR] Trying to delete null object." << std::endl;
            return false;
        }

        return true;
    }

    void Renderer_OpenGL::Destroy(VertexBuffer* pObj)
    {
        if (Validate(pObj))
        {
            ExecuteFunction([pObj]() { delete pObj; });
        }
    }
    void Renderer_OpenGL::Destroy(Shader* pShader)
    {
        if (Validate(pShader))
        {
            ExecuteFunction([pShader]() { delete pShader; });
        }
    }
    void Renderer_OpenGL::Destroy(Texture* pTex)
    {
        if (Validate(pTex))
        {
            ExecuteFunction([pTex]() { delete pTex; });
        }
    }
    void Renderer_OpenGL::Destroy(ShaderProgram* pProgram)
    {
        if (Validate(pProgram))
        {
            ExecuteFunction([pProgram]() { delete pProgram; });
        }
    }
} // namespace nate::Modules::Render
