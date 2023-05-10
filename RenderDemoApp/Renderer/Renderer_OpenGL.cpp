#include "Renderer_OpenGL.h"

#include "../3D/OpenGL_Object3D.h"
#include "../Shader/OpenGL_Shader.h"
#include "../Shader/OpenGL_ShaderProgram.h"
#include "../Texture/OpenGL_Texture.h"
#include "DebugCast.hpp"
#include "Window_GLFW.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <memory>
#include <stdexcept>

namespace nate::Modules::Render
{
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
                std::make_unique<OpenGL_Object3D>(std::move(vertexes), std::move(indeces)).release(),
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
            rslt =
                std::shared_ptr<Texture>(std::make_unique<OpenGL_Texture>(path, unit).release(), [this](Texture* pTex) {
                    Destroy(pTex);
                });
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

    GUI::IWindow* Renderer_OpenGL::Initialize(const GUI::WindowSize& size, std::string name)
    {
        ExecuteFunction([&]() -> void {
            m_pWin = std::unique_ptr<GUI::Window_GLFW, std::function<void(GUI::IWindow*)>>(
                std::make_unique<GUI::Window_GLFW>(size, std::move(name)).release(),
                [](GUI::IWindow* pWin) { delete pWin; });

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

        return m_pWin.get();
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
        ExecuteFunction([this]() -> void {
            glfwSwapBuffers(this->m_pWin->GetGLFWWindow());
            m_pWin->PollEvents();
        });
    }

    void Renderer_OpenGL::Destroy(GUI::IWindow* pWin)
    {
        if (!pWin)
        {
            return;
        }
        ExecuteFunction([pWin]() { delete pWin; });
    }

    void Renderer_OpenGL::Destroy(Object3D* pObj)
    {
        if (!pObj)
        {
            return;
        }
        ExecuteFunction([pObj]() { delete pObj; });
    }
    void Renderer_OpenGL::Destroy(Shader* pShader)
    {
        if (!pShader)
        {
            return;
        }
        ExecuteFunction([pShader]() { delete pShader; });
    }
    void Renderer_OpenGL::Destroy(Texture* pTex)
    {
        if (!pTex)
        {
            return;
        }
        ExecuteFunction([pTex]() { delete pTex; });
    }
    void Renderer_OpenGL::Destroy(ShaderProgram* pProgram)
    {
        if (!pProgram)
        {
            return;
        }
        ExecuteFunction([pProgram]() { delete pProgram; });
    }
} // namespace nate::Modules::Render
