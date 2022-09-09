#pragma once

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_EV_ViewUpdated : public sc::event<RendererSM_EV_ViewUpdated>
    {
      public:
        RendererSM_EV_ViewUpdated()  = default;
        ~RendererSM_EV_ViewUpdated() = default;
    };
} // namespace nate::Modules::Render