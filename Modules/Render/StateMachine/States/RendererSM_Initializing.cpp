#include "RendererSM_Initializing.h"

#include <bgfx/bgfx.h>

namespace nate::Modules::Render
{
    RendererSM_Initializing::RendererSM_Initializing(my_context ctx)
        : my_base(ctx)
    {
        bgfx::PlatformData platformData;

        platformData.nwh     = context<RendererSM>().Window()->NativeHandle();
        GUI::WindowSize size = context<RendererSM>().Window()->QueryWindowSize();

        bgfx::Init init;
        init.type              = bgfx::RendererType::Count;
        init.platformData      = platformData;
        init.resolution.width  = size.Width();
        init.resolution.height = size.Height();
        init.resolution.reset  = BGFX_RESET_VSYNC;

        if (!bgfx::init(init))
        {
            context<RendererSM>().PostEvent(RendererSM_EV_InitializationFailed());
            return;
        }

        const bgfx::ViewId kClearView = 0;
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x6495EDFF, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, size.Width(), size.Height());

        context<RendererSM>().PostEvent(RendererSM_EV_InitializationComplete());
    }
} // namespace nate::Modules::Render