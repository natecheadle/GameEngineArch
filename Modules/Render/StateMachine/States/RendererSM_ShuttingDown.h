#pragma once

#include "StateMachine/RendererSM.h"

#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_EV_ShutdownComplete;
    class RendererSM_Shutdown;

    class RendererSM_ShuttingDown : public sc::simple_state<RendererSM_ShuttingDown, RendererSM>
    {
      public:
        RendererSM_ShuttingDown();
        ~RendererSM_ShuttingDown() = default;

        typedef sc::transition<RendererSM_EV_ShutdownComplete, RendererSM_Shutdown> reactions;
    };
} // namespace nate::Modules::Render

#include "RendererSM_Shutdown.h"
#include "StateMachine/Events/RendererSM_EV_ShutdownComplete.hpp"
