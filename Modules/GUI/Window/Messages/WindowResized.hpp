#pragma once

#include "../WindowMessages.hpp"
#include "../WindowSize.hpp"

#include <SmallDataMessage.hpp>

namespace Ignosi::Modules::GUI {
    class WindowResized : public Messaging::SmallDataMessage<WindowMessages, WindowSize*> {
      public:
        WindowResized(WindowSize* size)
            : Messaging::SmallDataMessage<WindowMessages, WindowSize*>(WindowMessages::WindowResized, size)
        {
        }
    };
} // namespace Ignosi::Modules::GUI
