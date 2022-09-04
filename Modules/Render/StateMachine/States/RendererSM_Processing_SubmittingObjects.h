#pragma once

#include "StateMachine/RendererSM.h"

#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_Processing;
    class RendererSM_EV_ObjectSubmissionFailed;
    class RendererSM_EV_ObjectsSubmitted;
    class RendererSM_Processing_WaitingForFrame;
    class RendererSM_EV_ShutdownRequested;
    class RendererSM_ShuttingDown;

    class RendererSM_Processing_SubmittingObjects
        : public sc::simple_state<RendererSM_Processing_SubmittingObjects, RendererSM_Processing>
    {
      public:
        RendererSM_Processing_SubmittingObjects();
        ~RendererSM_Processing_SubmittingObjects() = default;

        typedef mpl::list<
            sc::transition<RendererSM_EV_ObjectSubmissionFailed, RendererSM_Error>,
            sc::transition<RendererSM_EV_ObjectsSubmitted, RendererSM_Processing_WaitingForFrame>,
            sc::transition<RendererSM_EV_ShutdownRequested, RendererSM_ShuttingDown>>
            reactions;
    };
} // namespace nate::Modules::Render

#include "RendererSM_Processing.h"
#include "RendererSM_Processing_WaitingForFrame.h"
#include "RendererSM_ShuttingDown.h"
#include "StateMachine/Events/RendererSM_EV_ObjectSubmissionFailed.hpp"
#include "StateMachine/Events/RendererSM_EV_ObjectsSubmitted.hpp"
#include "StateMachine/Events/RendererSM_EV_ShutdownRequested.hpp"