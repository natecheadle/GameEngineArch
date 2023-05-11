#include "Renderer_OpenGL.h"

#include "../3D/OpenGL_Object3D.h"
#include "../Shader/OpenGL_Shader.h"
#include "../Shader/OpenGL_ShaderProgram.h"
#include "../Texture/OpenGL_Texture.h"
#include "DebugCast.hpp"
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

    std::shared_ptr<Object3D> Renderer_OpenGL::CreateObject(std::vector<VertexData> vertexes)
    {
        std::shared_ptr<Object3D> rslt;
        ExecuteFunction([&]() -> void {
            rslt = std::shared_ptr<Object3D>(
                std::make_unique<OpenGL_Object3D>(std::move(vertexes)).release(),
                [this](Object3D* pObj) { Destroy(pObj); });
        });
        return rslt;
    }
    std::shared_ptr<Object3D> Renderer_OpenGL::CreateObject(
        std::vector<VertexData>    vertexes,
        std::vector<std::uint32_t> indeces)
    {
        std::shared_ptr<Object3D> rslt;
        ExecuteFunction([&]() -> void {
            rslt = std::shared_ptr<Object3D>(
                new OpenGL_Object3D(std::move(vertexes), std::move(indeces)),
                [this](Object3D* pObj) { Destroy(pObj); });
        });
        return rslt;
    }

    std::shared_ptr<Shader> Renderer_OpenGL::CreateShader(const std::filesystem::path& path)
    {
        std::shared_ptr<Shader> rslt;
        ExecuteFunction([&]() -> void {
            rslt = std::shared_ptr<Shader>(OpenGL_Shader::Create(path).release(), [this](Shader* pShader) {
                Destroy(pShader);
            });
        });
        return rslt;
    }

    std::shared_ptr<Shader> Renderer_OpenGL::CreateShader(const std::filesystem::path& path, ShaderType type)
    {
        std::shared_ptr<Shader> rslt;
        ExecuteFunction([&]() -> void {
            rslt = std::shared_ptr<Shader>(OpenGL_Shader::Create(path, type).release(), [this](Shader* pShader) {
                Destroy(pShader);
            });
        });
        return rslt;
    }

    std::shared_ptr<ShaderProgram> Renderer_OpenGL::CreateShaderProgram(
        const Shader* pFragmentShader,
        const Shader* pGeometryShader,
        const Shader* pVertexShader)
    {
        std::shared_ptr<ShaderProgram> rslt;
        ExecuteFunction([&]() -> void {
            rslt = std::shared_ptr<ShaderProgram>(
                std::make_unique<OpenGL_ShaderProgram>(pFragmentShader, pGeometryShader, pVertexShader).release(),
                [this](ShaderProgram* pProgram) { Destroy(pProgram); });
        });
        return rslt;
    }

    std::shared_ptr<Texture> Renderer_OpenGL::CreateTexture(const std::filesystem::path& path, TextureUnit unit)
    {
        std::shared_ptr<Texture> rslt;
        ExecuteFunction([&]() -> void {
            rslt = std::shared_ptr<Texture>(new OpenGL_Texture(path, unit), [this](Texture* pTex) { Destroy(pTex); });
        });
        return rslt;
    }

    std::shared_ptr<Texture> Renderer_OpenGL::CreateTexture(const ImageFile& image, TextureUnit unit)
    {
        std::shared_ptr<Texture> rslt;
        ExecuteFunction([&]() -> void {
            rslt = std::shared_ptr<Texture>(
                std::make_unique<OpenGL_Texture>(image, unit).release(),
                [this](Texture* pTex) { Destroy(pTex); });
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

    void Renderer_OpenGL::Destroy(Object3D* pObj)
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
