#pragma once

#include "StateMachine/RendererSM.h"

#include <boost/statechart/simple_state.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_Shutdown : public sc::simple_state<RendererSM_Shutdown, RendererSM>
    {
      public:
        RendererSM_Shutdown()  = default;
        ~RendererSM_Shutdown() = default;
    };
} // namespace nate::Modules::Render
