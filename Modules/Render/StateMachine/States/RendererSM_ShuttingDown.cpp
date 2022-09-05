#include "RendererSM_ShuttingDown.h"

#include <bgfx/bgfx.h>

namespace nate::Modules::Render
{
    RendererSM_ShuttingDown::RendererSM_ShuttingDown(my_context ctx)
        : my_base(ctx)
    {
        bgfx::shutdown();
        context<RendererSM>().PostEvent(RendererSM_EV_ShutdownComplete());
    }
} // namespace nate::Modules::Render