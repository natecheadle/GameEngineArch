#pragma once

#include "../MouseClickedInfo.hpp"
#include "../WindowMessages.hpp"

#include <Security/SecACL.h>
#include <SmallDataMessage.hpp>

namespace nate::Modules::GUI {
    class MouseClicked : public Messaging::SmallDataMessage<WindowMessages, MouseClickedInfo*> {
      public:
        MouseClicked(MouseClickedInfo* mouseClickedInfo)
            : Messaging::SmallDataMessage<WindowMessages, MouseClickedInfo*>(
                  WindowMessages::MouseClicked,
                  mouseClickedInfo)
        {
        }
    };
} // namespace nate::Modules::GUI
