#pragma once

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_EV_DrawFrame : public sc::event<RendererSM_EV_DrawFrame>
    {
      public:
        RendererSM_EV_DrawFrame()  = default;
        ~RendererSM_EV_DrawFrame() = default;
    };
} // namespace nate::Modules::Render