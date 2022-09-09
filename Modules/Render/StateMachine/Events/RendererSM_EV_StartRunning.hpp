#pragma once

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_EV_StartRunning : public sc::event<RendererSM_EV_StartRunning>
    {
      public:
        RendererSM_EV_StartRunning()  = default;
        ~RendererSM_EV_StartRunning() = default;
    };
} // namespace nate::Modules::Render