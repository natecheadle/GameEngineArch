#pragma once

namespace nate::Modules::GUI {
    enum class WindowMessages
    {
        WindowResized,
        WindowClosed,
        KeyPressed,
        MouseClicked,
    };
} // namespace nate::Modules::GUI