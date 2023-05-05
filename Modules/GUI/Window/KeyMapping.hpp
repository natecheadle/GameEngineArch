#include "KeyModifiers.hpp"
#include "Keys.h"

#include <array>
#include <functional>
#include <utility>

namespace nate::Modules::GUI
{
    class KeyMapping
    {
        MouseButton                                 m_MouseClickMapping{MouseButton::None};
        std::array<std::pair<Key, KeyModifiers>, 2> m_KeyMappings{
            {{Key::None, KeyModifiers()}, {Key::None, KeyModifiers()}}
        };

        std::function<void()> m_ExecuteKeyMappingEvent;

      public:
        KeyMapping(std::function<void()> keyEventFunc)
            : m_ExecuteKeyMappingEvent(std::move(keyEventFunc))
        {
        }

        KeyMapping(
            MouseButton                                        mouseClickMapping,
            const std::array<std::pair<Key, KeyModifiers>, 2>& keyMappings,
            std::function<void()>                              keyEventFunc)
            : m_MouseClickMapping(mouseClickMapping)
            , m_KeyMappings(keyMappings)
            , m_ExecuteKeyMappingEvent(std::move(keyEventFunc))
        {
        }

        const std::array<std::pair<Key, KeyModifiers>, 2>& KeyMappings() const { return m_KeyMappings; }
        MouseButton                                        MouseClickMapping() const { return m_MouseClickMapping; }

        void KeyMappings(const std::array<std::pair<Key, KeyModifiers>, 2>& value) { m_KeyMappings = value; }
        void MouseClickMapping(MouseButton value) { m_MouseClickMapping = value; }

        bool IsMappedKey(Key key, KeyModifiers mods) const
        {

            return (m_KeyMappings[0].first != Key::None && m_KeyMappings[0].first == key &&
                    m_KeyMappings[0].second == mods) ||
                   (m_KeyMappings[1].first != Key::None && m_KeyMappings[1].first == key &&
                    m_KeyMappings[1].second == mods);
        }

        void AttachKeyEvent(std::function<void()> func) { m_ExecuteKeyMappingEvent = std::move(func); }
        void ExecuteKeyEvent() const { m_ExecuteKeyMappingEvent(); }
    };
} // namespace nate::Modules::GUI