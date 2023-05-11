#pragma once

#include "../KeyPressedInfo.hpp"
#include "../WindowMessages.hpp"

#include <SmallDataMessage.hpp>

#include <memory>

namespace nate::Modules::GUI {
    class KeyPressed : public Messaging::SmallDataMessage<WindowMessages, KeyPressedInfo*> {
      public:
        KeyPressed(KeyPressedInfo* keyPressInfo)
            : Messaging::SmallDataMessage<WindowMessages, KeyPressedInfo*>(WindowMessages::KeyPressed, keyPressInfo)
        {
        }
    };
} // namespace nate::Modules::GUI
