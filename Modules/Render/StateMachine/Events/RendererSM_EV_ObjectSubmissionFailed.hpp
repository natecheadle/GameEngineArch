#pragma once

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

namespace nate::Modules::Render
{
    class RendererSM_EV_ObjectSubmissionFailed : public sc::event<RendererSM_EV_ObjectSubmissionFailed>
    {
      public:
        RendererSM_EV_ObjectSubmissionFailed()  = default;
        ~RendererSM_EV_ObjectSubmissionFailed() = default;
    };
} // namespace nate::Modules::Render