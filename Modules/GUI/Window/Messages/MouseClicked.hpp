#pragma once

#include "../MouseClickedInfo.hpp"
#include "../WindowMessages.hpp"

#include <SmallDataMessage.hpp>

namespace Ignosi::Modules::GUI {
    class MouseClicked : public Messaging::SmallDataMessage<WindowMessages, MouseClickedInfo*> {
      public:
        MouseClicked(MouseClickedInfo* mouseClickedInfo)
            : Messaging::SmallDataMessage<WindowMessages, MouseClickedInfo*>(
                  WindowMessages::MouseClicked,
                  mouseClickedInfo)
        {
        }
    };
} // namespace Ignosi::Modules::GUI
