#pragma once

#include "StateMachine/RendererSM.h"

#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_Processing;
    class RendererSM_EV_ViewConfigurationFailed;
    class RendererSM_EV_ViewUpdated;
    class RendererSM_Processing_SubmittingObjects;
    class RendererSM_EV_ShutdownRequested;
    class RendererSM_ShuttingDown;

    class RendererSM_Processing_ConfiguringView
        : public sc::state<RendererSM_Processing_ConfiguringView, RendererSM_Processing>
    {
      public:
        RendererSM_Processing_ConfiguringView(my_context ctx);
        ~RendererSM_Processing_ConfiguringView() = default;

        typedef mpl::list<
            sc::transition<RendererSM_EV_ViewConfigurationFailed, RendererSM_Error>,
            sc::transition<RendererSM_EV_ViewUpdated, RendererSM_Processing_SubmittingObjects>,
            sc::transition<RendererSM_EV_ShutdownRequested, RendererSM_ShuttingDown>>
            reactions;
    };
} // namespace nate::Modules::Render

#include "RendererSM_Error.h"
#include "RendererSM_Processing_SubmittingObjects.h"
#include "RendererSM_ShuttingDown.h"
#include "StateMachine/Events/RendererSM_EV_ShutdownRequested.hpp"
#include "StateMachine/Events/RendererSM_EV_ViewConfigurationFailed.hpp"
#include "StateMachine/Events/RendererSM_EV_ViewUpdated.hpp"
