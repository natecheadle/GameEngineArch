#include "Renderer.h"

#include "CursorPosition.hpp"
#include "Keys.h"
#include "Matrix4x4.h"
#include "Shader/BGFX_Shader.h"
#include "WindowMessages.hpp"
#include "WindowSize.hpp"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>

#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <thread>

namespace nate::Modules::Render {

    namespace {
        struct PosColorVertex
        {
            float    x;
            float    y;
            float    z;
            uint32_t abgr;
        };

    } // namespace

    void Renderer::Initialize(GUI::IWindow* pWindow, std::filesystem::path shaderLoc)
    {
        // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
        // Most graphics APIs must be used on the same thread that created the window.
        bgfx::renderFrame();
        m_pWindow   = pWindow;
        m_ShaderLoc = std::move(shaderLoc);
        Start();
    }

    void Renderer::RenderFrame()
    {
        bgfx::renderFrame();
    }

    void Renderer::ExecuteJob()
    {
        bgfx::PlatformData platformData;
        m_pWindow->SubscribeToMessage(this, GUI::WindowMessages::WindowClosed, [this](const GUI::WindowMessage*) {
            m_WindowShouldClose = true;
        });

        platformData.nwh     = m_pWindow->NativeHandle();
        GUI::WindowSize size = m_pWindow->QueryWindowSize();

        bgfx::Init init;
        init.type              = bgfx::RendererType::Count;
        init.platformData      = platformData;
        init.resolution.width  = size.Width();
        init.resolution.height = size.Height();
        init.resolution.reset  = BGFX_RESET_VSYNC;

        if (!bgfx::init(init))
            return;

        const bgfx::ViewId kClearView = 0;
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x6495EDFF, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, size.Width(), size.Height());

        // TODO shaders should be part of the material attached to the object
        Render::BGFX_Shader fragmentShader("fs_cubes.sc.bin", m_ShaderLoc);
        Render::BGFX_Shader vertexShader("vs_cubes.sc.bin", m_ShaderLoc);
        bgfx::ProgramHandle program = bgfx::createProgram(vertexShader.Handle(), fragmentShader.Handle(), true);

        m_RendererInitialized = true;

        while (!ShouldStop() && !m_WindowShouldClose)
        {
            GUI::WindowSize size = m_pWindow->QueryWindowSize();

            if (m_pCamera)
            {
                Matrix4x4 proj = m_pCamera->CreateProjection(size.Width(), size.Height());
                Matrix4x4 view = m_pCamera->View();
                view.Invert();
                bgfx::setViewTransform(0, view.Data().data(), proj.Data().data());
            }

            while (!m_Objects.empty())
            {
                const Object3D* pObject = m_Objects.front();
                m_Objects.pop();
                if (!pObject)
                    continue;

                bgfx::setTransform(pObject->Transformation().Data().data());

                bgfx::setVertexBuffer(0, pObject->VertexBufferHandle());
                bgfx::setIndexBuffer(pObject->IndexBufferHandle());

                bgfx::submit(0, program);
            }

            bgfx::frame();
        }

        bgfx::destroy(program);

        m_WaitingForShutdown = true;
        while (!m_ShouldShutdown)
        {
            std::this_thread::yield();
        }

        bgfx::shutdown();
    }

    void Renderer::PrivShutdown()
    {
        Stop();
        m_ShouldShutdown = true;

        while (IsRunning())
        {
            bgfx::renderFrame();
            Join(std::chrono::milliseconds(10));
        }

        m_pWindow->Unsubscribe(this);
    }
} // namespace nate::Modules::Render