#include "RendererSM_Processing_ConfiguringView.h"

#include "StateMachine/RendererSM.h"

#include <cstddef>
#include <thread>


namespace nate::Modules::Render
{
    RendererSM_Processing_ConfiguringView::RendererSM_Processing_ConfiguringView(my_context ctx)
        : my_base(std::move(ctx))
    {
        context<RendererSM>().WaitSubmission();
        GUI::WindowSize size = context<RendererSM>().Window()->QueryWindowSize();

        std::shared_ptr<const Camera3D> pCamera = context<RendererSM>().GetCamera();
        Matrix4x4                       proj    = pCamera->CreateProjection(size.Width(), size.Height());
        Matrix4x4                       view    = pCamera->View();
        view.Invert();
        bgfx::setViewTransform(0, view.Data().data(), proj.Data().data());

        context<RendererSM>().PostEvent(RendererSM_EV_ViewUpdated());
    }
} // namespace nate::Modules::Render