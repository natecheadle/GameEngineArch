#pragma once

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_EV_ShutdownRequested : public sc::event<RendererSM_EV_ShutdownRequested>
    {
      public:
        RendererSM_EV_ShutdownRequested()  = default;
        ~RendererSM_EV_ShutdownRequested() = default;
    };
} // namespace nate::Modules::Render