#pragma once

#include "StateMachine/RendererSM.h"

#include <boost/statechart/simple_state.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_Error : public sc::simple_state<RendererSM_Error, RendererSM>
    {
      public:
        RendererSM_Error()  = default;
        ~RendererSM_Error() = default;
    };
} // namespace nate::Modules::Render
