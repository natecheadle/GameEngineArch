#pragma once

#include "WindowMessages.hpp"

#include <MessagePump.hpp>

namespace nate::Modules::GUI
{
    class WindowMessagePump : public Messaging::MessagePump<WindowMessages>
    {
    };
} // namespace nate::Modules::GUI