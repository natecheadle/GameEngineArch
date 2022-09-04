#pragma once

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_EV_InitializationComplete : public sc::event<RendererSM_EV_InitializationComplete>
    {
      public:
        RendererSM_EV_InitializationComplete()  = default;
        ~RendererSM_EV_InitializationComplete() = default;
    };
} // namespace nate::Modules::Render