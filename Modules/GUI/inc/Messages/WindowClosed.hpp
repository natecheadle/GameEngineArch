#pragma once

#include "../WindowMessages.hpp"

#include <Message.hpp>

namespace nate::Modules::GUI {
    class WindowClosed : public Messaging::Message<WindowMessages> {
      public:
        WindowClosed()
            : Messaging::Message<WindowMessages>(WindowMessages::WindowClosed)
        {
        }
    };
} // namespace nate::Modules::GUI
