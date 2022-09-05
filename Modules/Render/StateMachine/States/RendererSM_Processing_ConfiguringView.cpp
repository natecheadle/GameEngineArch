#include "RendererSM_Processing_ConfiguringView.h"

namespace nate::Modules::Render
{
    RendererSM_Processing_ConfiguringView::RendererSM_Processing_ConfiguringView(my_context ctx)
        : my_base(std::move(ctx))
    {
        GUI::WindowSize size = context<RendererSM>().Window()->QueryWindowSize();

        auto cameraPair = context<RendererSM>().GetCamera();
        if (cameraPair.first)
        {
            Matrix4x4 proj = cameraPair.first->CreateProjection(size.Width(), size.Height());
            Matrix4x4 view = cameraPair.first->View();
            view.Invert();
            bgfx::setViewTransform(0, view.Data().data(), proj.Data().data());
        }

        context<RendererSM>().PostEvent(RendererSM_EV_ViewUpdated());
    }
} // namespace nate::Modules::Render