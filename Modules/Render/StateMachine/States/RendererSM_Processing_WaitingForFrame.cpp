#include "RendererSM_Processing_WaitingForFrame.h"

#include <bgfx/bgfx.h>

namespace nate::Modules::Render
{
    RendererSM_Processing_WaitingForFrame::RendererSM_Processing_WaitingForFrame(my_context ctx)
        : my_base(std::move(ctx))
    {
        bgfx::frame();
        context<RendererSM>().PostEvent(RendererSM_EV_FrameRenderered());
    }
} // namespace nate::Modules::Render