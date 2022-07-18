#pragma once

#include "../WindowMessages.hpp"
#include "../WindowSize.hpp"

#include <SmallDataMessage.hpp>

namespace nate::Modules::GUI {
    class WindowResized : public Messaging::SmallDataMessage<WindowMessages, WindowSize*> {
      public:
        WindowResized(WindowSize* size)
            : Messaging::SmallDataMessage<WindowMessages, WindowSize*>(WindowMessages::WindowResized, size)
        {
        }
    };
} // namespace nate::Modules::GUI
