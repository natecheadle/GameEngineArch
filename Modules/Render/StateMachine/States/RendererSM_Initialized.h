#pragma once

#pragma once

#include "StateMachine/RendererSM.h"

#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_EV_StartRunning;
    class RendererSM_Processing;

    class RendererSM_Initialized : public sc::state<RendererSM_Initialized, RendererSM>
    {
      public:
        RendererSM_Initialized(my_context ctx);
        ~RendererSM_Initialized() = default;

        typedef sc::transition<RendererSM_EV_StartRunning, RendererSM_Processing> reactions;

      private:
        void BuildShaders();
        void BuildPrograms();
    };
} // namespace nate::Modules::Render

#include "RendererSM_Processing.h"
#include "StateMachine/Events/RendererSM_EV_StartRunning.hpp"
