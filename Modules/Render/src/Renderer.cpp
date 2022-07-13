#include "Renderer.h"

#include "WindowSize.hpp"
#include "logo.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>

namespace nate::Modules::Render {
    void Renderer::Initialize(GUI::IWindow* pWindow)
    {
        // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
        // Most graphics APIs must be used on the same thread that created the window.
        bgfx::renderFrame();
        m_pWindow = pWindow;
        Start();
    }
    void Renderer::RenderFrame()
    {
        bgfx::renderFrame();
    }

    void Renderer::ExecuteJob()
    {
        bgfx::PlatformData platformData;
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
        bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
        bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

        while (!ShouldStop() && !m_pWindow->ShouldClose())
        {
            bgfx::touch(kClearView);
            // Use debug font to print information about this example.
            bgfx::dbgTextClear();
            bgfx::dbgTextImage(
                bx::max<uint16_t>(uint16_t(size.Width() / 2 / 8), 20) - 20,
                bx::max<uint16_t>(uint16_t(size.Height() / 2 / 16), 6) - 6,
                40,
                12,
                s_logo,
                160);
            bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
            bgfx::dbgTextPrintf(
                0,
                1,
                0x0f,
                "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m "
                "code "
                "too.");
            bgfx::dbgTextPrintf(
                80,
                1,
                0x0f,
                "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    "
                "\x1b[0m");
            bgfx::dbgTextPrintf(
                80,
                2,
                0x0f,
                "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    "
                "\x1b[0m");
            const bgfx::Stats* stats = bgfx::getStats();
            bgfx::dbgTextPrintf(
                0,
                2,
                0x0f,
                "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.",
                stats->width,
                stats->height,
                stats->textWidth,
                stats->textHeight);
            // Enable stats or debug text.
            bgfx::setDebug(BGFX_DEBUG_STATS);
            // Advance to next frame. Main thread will be kicked to process submitted rendering primitives.
            bgfx::frame();
        }

        bgfx::shutdown();
    }
} // namespace nate::Modules::Render