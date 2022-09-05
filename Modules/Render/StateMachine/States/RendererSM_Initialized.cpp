#include "RendererSM_Initialized.h"

#include "StateMachine/RendererSM.h"

#include <mutex>

namespace nate::Modules::Render
{
    RendererSM_Initialized::RendererSM_Initialized(my_context ctx)
        : my_base(ctx)
    {
        std::unique_lock<std::mutex> lock = context<RendererSM>().AcquireStartRunningLock();
        context<RendererSM>().InitializationComplete();
        context<RendererSM>().WaitStartRunning(lock);
        context<RendererSM>().PostEvent(RendererSM_EV_StartRunning());
    }
} // namespace nate::Modules::Render
