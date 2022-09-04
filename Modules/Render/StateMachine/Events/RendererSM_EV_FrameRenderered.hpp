#pragma once

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_EV_FrameRenderered : public sc::event<RendererSM_EV_FrameRenderered>
    {
      public:
        RendererSM_EV_FrameRenderered()  = default;
        ~RendererSM_EV_FrameRenderered() = default;
    };
} // namespace nate::Modules::Render