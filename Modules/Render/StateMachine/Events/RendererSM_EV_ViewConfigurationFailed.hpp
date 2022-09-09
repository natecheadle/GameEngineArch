#pragma once

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_EV_ViewConfigurationFailed : public sc::event<RendererSM_EV_ViewConfigurationFailed>
    {
      public:
        RendererSM_EV_ViewConfigurationFailed()  = default;
        ~RendererSM_EV_ViewConfigurationFailed() = default;
    };
} // namespace nate::Modules::Render