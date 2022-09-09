#pragma once

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_EV_ObjectsSubmitted : public sc::event<RendererSM_EV_ObjectsSubmitted>
    {
      public:
        RendererSM_EV_ObjectsSubmitted()  = default;
        ~RendererSM_EV_ObjectsSubmitted() = default;
    };
} // namespace nate::Modules::Render