#include "Renderer.h"

#include "BGFX_Shader.h"
#include "CursorPosition.hpp"
#include "Keys.h"
#include "Matrix4x4.h"
#include "WindowMessages.hpp"
#include "WindowSize.hpp"

#include <DebugCast.hpp>
#include <Messages/MouseClicked.hpp>
#include <Messages/WindowResized.hpp>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>

#include <chrono>
#include <mutex>
#include <shared_mutex>

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
        m_pWindow    = pWindow;
        m_ShaderLoc  = std::move(shaderLoc);
        m_WindowSize = m_pWindow->QueryWindowSize();
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
        m_pWindow->SubscribeToMessage(
            this,
            GUI::WindowMessages::MouseClicked,
            [this](const GUI::WindowMessage* pMessage) {
                std::unique_lock<std::shared_mutex> lock(m_CallbackMutex);

                m_LastMouseClick = *(DebugCast<const GUI::MouseClicked*>(pMessage)->GetData());
            });
        m_pWindow->SubscribeToMessage(
            this,
            GUI::WindowMessages::WindowResized,
            [this](const GUI::WindowMessage* pMessage) {
                std::unique_lock<std::shared_mutex> lock(m_CallbackMutex);

                m_WindowSize = *(DebugCast<const GUI::WindowResized*>(pMessage)->GetData());
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

        bgfx::VertexLayout pos_col_vert_layout;
        pos_col_vert_layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
        bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(
            bgfx::makeRef(m_pObject->Points().data(), (uint32_t)m_pObject->PointsSize()),
            pos_col_vert_layout);
        bgfx::IndexBufferHandle ibh =
            bgfx::createIndexBuffer(bgfx::makeRef(m_pObject->Indices().data(), m_pObject->IndecesSize()));

        Render::BGFX_Shader fragmentShader("fs_cubes.sc.bin", m_ShaderLoc);
        Render::BGFX_Shader vertexShader("vs_cubes.sc.bin", m_ShaderLoc);
        bgfx::ProgramHandle program = bgfx::createProgram(vertexShader.Handle(), fragmentShader.Handle(), true);

        m_RendererInitialized = true;

        while (!ShouldStop() && !m_WindowShouldClose)
        {
            // simple input code for orbit camera
            GUI::KeyState    lastKeyState{GUI::KeyState::Released};
            GUI::MouseButton lastMouseButton{GUI::MouseButton::Left};
            int              width{0};
            int              height{0};

            {
                std::shared_lock<std::shared_mutex> lock(m_CallbackMutex);
                lastKeyState    = m_LastMouseClick.KeyState();
                lastMouseButton = m_LastMouseClick.Button();
                width           = m_WindowSize.Width();
                height          = m_WindowSize.Height();
            }
            if (lastKeyState == GUI::KeyState::Pressed && lastMouseButton == GUI::MouseButton::Left)
            {
                GUI::CursorPosition position = m_pWindow->QueryCursorPosition();

                static constexpr float rot_scale = 0.01;
                double                 delta_x   = position.XPos() - m_LastPosition.XPos();
                double                 delta_y   = position.YPos() - m_LastPosition.YPos();
                m_CamYaw += float(-delta_x) * rot_scale;
                m_CamPitch += float(-delta_y) * rot_scale;

                m_LastPosition = position;
            }
            Matrix4x4 view;
            view.SetToIdentity();
            view.Rotate(m_CamPitch, m_CamYaw, 0.0);
            view.Translate(0.0, 0.0, -5.0);
            view.Invert();

            Matrix4x4 proj;
            bx::mtxProj(
                proj.Data().data(),
                60.0f,
                float(width) / float(height),
                0.1f,
                100.0f,
                bgfx::getCaps()->homogeneousDepth);

            bgfx::setViewTransform(0, view.Data().data(), proj.Data().data());

            Matrix4x4 model;
            model.SetToIdentity();

            bgfx::setTransform(model.Data().data());

            bgfx::setVertexBuffer(0, vbh);
            bgfx::setIndexBuffer(ibh);

            bgfx::submit(0, program);

            bgfx::frame();
        }

        bgfx::destroy(vbh);
        bgfx::destroy(ibh);
        bgfx::destroy(program);

        bgfx::shutdown();
    }

    void Renderer::PrivShutdown()
    {
        Stop();
        while (IsRunning())
        {
            bgfx::renderFrame();
            Join(std::chrono::milliseconds(10));
        }

        m_pWindow->Unsubsribe(this);
    }
} // namespace nate::Modules::Render