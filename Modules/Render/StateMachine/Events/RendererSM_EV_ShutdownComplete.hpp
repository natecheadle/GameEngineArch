#pragma once

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_EV_ShutdownComplete : public sc::event<RendererSM_EV_ShutdownComplete>
    {
      public:
        RendererSM_EV_ShutdownComplete()  = default;
        ~RendererSM_EV_ShutdownComplete() = default;
    };
} // namespace nate::Modules::Render