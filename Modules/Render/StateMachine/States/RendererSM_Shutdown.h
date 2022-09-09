#pragma once

#include "StateMachine/RendererSM.h"

#include <boost/statechart/state.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_Shutdown : public sc::state<RendererSM_Shutdown, RendererSM>
    {
      public:
        RendererSM_Shutdown(my_context ctx)
            : my_base(ctx)
        {
            context<RendererSM>().ShutdownComplete();
        }
        ~RendererSM_Shutdown() = default;
    };
} // namespace nate::Modules::Render
