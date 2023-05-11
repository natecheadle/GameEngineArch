#pragma once

#include "Keys.h"

#include <array>
#include <bitset>

namespace nate::Modules::GUI {
    enum class KeyModifier
    {
        Shift,
        Control,
        Alt,
        Super,
        CapsLock,
        NumLock,

        LAST,
    };

    class KeyModifiers {
        std::bitset<static_cast<size_t>(KeyModifier::LAST)> m_Modifiers{0};

      public:
        KeyModifiers() = default;
        KeyModifiers(
            bool isShiftPressed,
            bool isControlPressed,
            bool isAltPressed,
            bool isSuperPressed,
            bool isCapsLockPressed,
            bool isNumLockPressed)
        {
            m_Modifiers[static_cast<size_t>(KeyModifier::Shift)]    = isShiftPressed;
            m_Modifiers[static_cast<size_t>(KeyModifier::Control)]  = isControlPressed;
            m_Modifiers[static_cast<size_t>(KeyModifier::Alt)]      = isAltPressed;
            m_Modifiers[static_cast<size_t>(KeyModifier::Super)]    = isSuperPressed;
            m_Modifiers[static_cast<size_t>(KeyModifier::CapsLock)] = isCapsLockPressed;
            m_Modifiers[static_cast<size_t>(KeyModifier::NumLock)]  = isNumLockPressed;
        }

        ~KeyModifiers() = default;

        KeyModifiers(const KeyModifiers& other)            = default;
        KeyModifiers& operator=(const KeyModifiers& other) = default;

        bool operator==(const KeyModifiers& other) const = default;

        bool IsShiftPressed() const { return m_Modifiers[static_cast<size_t>(KeyModifier::Shift)]; }
        bool IsControlPressed() const { return m_Modifiers[static_cast<size_t>(KeyModifier::Control)]; }
        bool IsAltPressed() const { return m_Modifiers[static_cast<size_t>(KeyModifier::Alt)]; }
        bool IsSuperPressed() const { return m_Modifiers[static_cast<size_t>(KeyModifier::Super)]; }
        bool IsCapsLockPressed() const { return m_Modifiers[static_cast<size_t>(KeyModifier::CapsLock)]; }
        bool IsNumLockPressed() const { return m_Modifiers[static_cast<size_t>(KeyModifier::NumLock)]; }
        bool IsAnyPressed() const { return m_Modifiers.any(); }
        bool IsModifierPressed(const KeyModifiers& other)
        {
            return (m_Modifiers.to_ulong() & other.m_Modifiers.to_ulong()) != 0;
        }
    };
} // namespace nate::Modules::GUI