#pragma once

#include "WindowMessages.hpp"

#include <MessagePump.hpp>

namespace Ignosi::Modules::GUI
{
    class WindowMessagePump : public Messaging::MessagePump<WindowMessages>
    {
    };
} // namespace Ignosi::Modules::GUI