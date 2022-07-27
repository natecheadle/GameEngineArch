#include "Renderer.h"

#include "BGFX_Shader.h"
#include "CursorPosition.hpp"
#include "Keys.h"
#include "WindowMessages.hpp"
#include "WindowSize.hpp"

#include <DebugCast.hpp>
#include <Messages/MouseClicked.hpp>
#include <Messages/WindowResized.hpp>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>

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

        PosColorVertex cube_vertices[] = {
            {-1.0f, 1.0f,  1.0f,  0xff000000},
            {1.0f,  1.0f,  1.0f,  0xff0000ff},
            {-1.0f, -1.0f, 1.0f,  0xff00ff00},
            {1.0f,  -1.0f, 1.0f,  0xff00ffff},
            {-1.0f, 1.0f,  -1.0f, 0xffff0000},
            {1.0f,  1.0f,  -1.0f, 0xffff00ff},
            {-1.0f, -1.0f, -1.0f, 0xffffff00},
            {1.0f,  -1.0f, -1.0f, 0xffffffff},
        };

        const uint16_t cube_tri_list[] = {
            0, 1, 2, 1, 3, 2, 4, 6, 5, 5, 6, 7, 0, 2, 4, 4, 2, 6, 1, 5, 3, 5, 7, 3, 0, 4, 1, 4, 5, 1, 2, 3, 6, 6, 3, 7,
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

    void Renderer::RenderObject(const Object3D* pObject)
    {
        std::unique_lock<std::mutex> lock(m_RenderObjectsMutex);
        m_ObjectsToRender.push(pObject);
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
        bgfx::VertexBufferHandle vbh =
            bgfx::createVertexBuffer(bgfx::makeRef(cube_vertices, sizeof(cube_vertices)), pos_col_vert_layout);
        bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(bgfx::makeRef(cube_tri_list, sizeof(cube_tri_list)));

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

            float cam_rotation[16];
            bx::mtxRotateXYZ(cam_rotation, m_CamPitch, m_CamYaw, 0.0f);

            float cam_translation[16];
            bx::mtxTranslate(cam_translation, 0.0f, 0.0f, -5.0f);

            float cam_transform[16];
            bx::mtxMul(cam_transform, cam_translation, cam_rotation);

            float view[16];
            bx::mtxInverse(view, cam_transform);

            float proj[16];
            bx::mtxProj(proj, 60.0f, float(width) / float(height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

            bgfx::setViewTransform(0, view, proj);

            float model[16];
            bx::mtxIdentity(model);
            bgfx::setTransform(model);

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
        m_pWindow->Unsubsribe(this);
    }
} // namespace nate::Modules::Render