#pragma once

#include "StateMachine/RendererSM.h"

#include <boost/statechart/simple_state.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_Processing_ConfiguringView;

    class RendererSM_Processing
        : public sc::simple_state<RendererSM_Processing, RendererSM, RendererSM_Processing_ConfiguringView>
    {
      public:
        RendererSM_Processing()  = default;
        ~RendererSM_Processing() = default;
    };
} // namespace nate::Modules::Render

#include "RendererSM_Processing_ConfiguringView.h"
