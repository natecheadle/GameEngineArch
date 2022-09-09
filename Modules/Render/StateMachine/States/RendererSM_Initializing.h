#pragma once

#include "StateMachine/RendererSM.h"

#include <boost/mpl/list.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>

namespace mpl = boost::mpl;
namespace sc  = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_EV_InitializationComplete;
    class RendererSM_Error;
    class RendererSM_Initialized;
    class RendererSM_StartInitialization;
    class RendererSM_EV_InitializationFailed;

    class RendererSM_Initializing : public sc::state<RendererSM_Initializing, RendererSM>
    {
      public:
        RendererSM_Initializing(my_context ctx);
        ~RendererSM_Initializing() = default;

        typedef mpl::list<
            sc::transition<RendererSM_EV_InitializationFailed, RendererSM_Error>,
            sc::transition<RendererSM_EV_InitializationComplete, RendererSM_Initialized>>
            reactions;
    };
} // namespace nate::Modules::Render

#include "RendererSM_Error.h"
#include "RendererSM_Initialized.h"
#include "StateMachine/Events/RendererSM_EV_InitializationComplete.hpp"
#include "StateMachine/Events/RendererSM_EV_InitializationFailed.hpp"
