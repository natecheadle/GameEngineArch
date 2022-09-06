#include "RendererSM_Processing_ConfiguringView.h"

#include <cstddef>
#include <thread>

namespace nate::Modules::Render
{
    RendererSM_Processing_ConfiguringView::RendererSM_Processing_ConfiguringView(my_context ctx)
        : my_base(std::move(ctx))
    {
        GUI::WindowSize size     = context<RendererSM>().Window()->QueryWindowSize();
        bool            noCamera = false;
        do
        {
            std::pair<std::shared_ptr<const Camera3D>, std::unique_lock<std::mutex>> cameraPair =
                context<RendererSM>().GetCamera();
            noCamera = cameraPair.first == nullptr;
            if (noCamera)
                std::this_thread::yield();
            else
            {
                Matrix4x4 proj = cameraPair.first->CreateProjection(size.Width(), size.Height());
                Matrix4x4 view = cameraPair.first->View();
                view.Invert();
                bgfx::setViewTransform(0, view.Data().data(), proj.Data().data());
            }
        } while (noCamera);

        context<RendererSM>().PostEvent(RendererSM_EV_ViewUpdated());
    }
} // namespace nate::Modules::Render