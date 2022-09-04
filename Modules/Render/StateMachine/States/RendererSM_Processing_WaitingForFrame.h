#pragma once

#include "StateMachine/RendererSM.h"

#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_Processing;
    class RendererSM_EV_FrameRenderered;
    class RendererSM_Processing_ConfiguringView;
    class RendererSM_EV_ShutdownRequested;
    class RendererSM_ShuttingDown;

    class RendererSM_Processing_WaitingForFrame
        : public sc::simple_state<RendererSM_Processing_WaitingForFrame, RendererSM_Processing>
    {
      public:
        RendererSM_Processing_WaitingForFrame();
        ~RendererSM_Processing_WaitingForFrame() = default;

        typedef mpl::list<
            sc::transition<RendererSM_EV_FrameRenderered, RendererSM_Processing_ConfiguringView>,
            sc::transition<RendererSM_EV_ShutdownRequested, RendererSM_ShuttingDown>>
            reactions;
    };
} // namespace nate::Modules::Render

#include "RendererSM_Processing.h"
#include "RendererSM_Processing_ConfiguringView.h"
#include "RendererSM_ShuttingDown.h"
#include "StateMachine/Events/RendererSM_EV_FrameRenderered.hpp"
#include "StateMachine/Events/RendererSM_EV_ShutdownRequested.hpp"
