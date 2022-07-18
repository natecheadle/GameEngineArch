#pragma once

#include "KeyModifiers.hpp"
#include "Keys.h"

namespace nate::Modules::GUI {
    class KeyPressedInfo {
        Key          m_Key{Key::A};
        KeyModifiers m_KeyMods;
        KeyState     m_KeyState{KeyState::Released};
        int          m_ScanCode;

      public:
        KeyPressedInfo() = default;
        KeyPressedInfo(Key key, KeyModifiers keyMods, KeyState keyState, int scanCode)
            : m_Key(key)
            , m_KeyMods(keyMods)
            , m_KeyState(keyState)
            , m_ScanCode(scanCode)
        {
        }
        ~KeyPressedInfo() = default;

        KeyPressedInfo(const KeyPressedInfo& other)            = default;
        KeyPressedInfo& operator=(const KeyPressedInfo& other) = default;

        bool operator==(const KeyPressedInfo& other) const = default;

        Key          Key() const { return m_Key; }
        KeyModifiers KeyMods() const { return m_KeyMods; }
        KeyState     KeyState() const { return m_KeyState; }
        int          ScanCode() const { return m_ScanCode; }

        void Key(enum Key value) { m_Key = value; }
        void KeyMods(KeyModifiers value) { m_KeyMods = value; }
        void KeyState(enum KeyState value) { m_KeyState = value; }
        void ScanCode(int value) { m_ScanCode = value; }
    };
} // namespace nate::Modules::GUI