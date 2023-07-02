#pragma once

#include "KeyModifiers.hpp"
#include "Keys.h"

namespace Ignosi::Modules::GUI {
    class MouseClickedInfo {
        MouseButton  m_Button{MouseButton::Left};
        KeyModifiers m_KeyMods;
        KeyState     m_KeyState{KeyState::Released};

      public:
        MouseClickedInfo() = default;
        MouseClickedInfo(MouseButton button, KeyModifiers keyMods, KeyState keyState)
            : m_Button(button)
            , m_KeyMods(keyMods)
            , m_KeyState(keyState)
        {
        }
        ~MouseClickedInfo() = default;

        MouseClickedInfo(const MouseClickedInfo& other)            = default;
        MouseClickedInfo& operator=(const MouseClickedInfo& other) = default;

        bool operator==(const MouseClickedInfo& other) const = default;

        MouseButton  Button() const { return m_Button; }
        KeyModifiers KeyMods() const { return m_KeyMods; }
        KeyState     KeyState() const { return m_KeyState; }

        void Button(MouseButton value) { m_Button = value; }
        void KeyMods(KeyModifiers value) { m_KeyMods = value; }
        void KeyState(enum KeyState value) { m_KeyState = value; }
    };
} // namespace Ignosi::Modules::GUI