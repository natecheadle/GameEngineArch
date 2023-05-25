#pragma once

#include <cstdint>
namespace nate::Modules::GUI
{
    enum class MouseButton : std::uint16_t
    {
        None,
        Left,
        Middle,
        Right,

        LAST
    };

    enum class KeyState : std::uint16_t
    {
        Released,
        Pressed,
        Repeat,

        LAST
    };

    enum class Key : std::uint16_t
    {
        None,

        // Numbers
        Zero,
        One,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,

        // Letters
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        // Arrow Keys
        Up,
        Down,
        Left,
        Right,

        // Function Keys
        Escape,
        Enter,
        Tab,
        Backspace,
        Insert,
        Delete,
        Shift,
        Control,
        Alt,
        Super,
        CapsLock,
        NumLock,
        Space,

        LAST
    };
} // namespace nate::Modules::GUI
