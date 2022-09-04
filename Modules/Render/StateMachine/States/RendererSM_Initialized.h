#pragma once

#pragma once

#include "StateMachine/RendererSM.h"

#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_EV_StartRunning;
    class RendererSM_Processing;

    class RendererSM_Initialized : public sc::simple_state<RendererSM_Initialized, RendererSM>
    {
      public:
        RendererSM_Initialized()  = default;
        ~RendererSM_Initialized() = default;

        typedef sc::transition<RendererSM_EV_StartRunning, RendererSM_Processing> reactions;
    };
} // namespace nate::Modules::Render

#include "RendererSM_Processing.h"
#include "StateMachine/Events/RendererSM_EV_StartRunning.hpp"
