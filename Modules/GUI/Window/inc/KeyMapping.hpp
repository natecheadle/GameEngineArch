#include "KeyModifiers.hpp"
#include "Keys.h"

#include <array>
#include <utility>

namespace nate::Modules::GUI {
    class KeyMapping {
        MouseButton                                 m_MouseClickMapping{MouseButton::None};
        std::array<std::pair<Key, KeyModifiers>, 2> m_KeyMappings{
            {{Key::None, KeyModifiers()}, {Key::None, KeyModifiers()}}
        };

      public:
        KeyMapping() = default;
        KeyMapping(MouseButton mouseClickMapping, const std::array<std::pair<Key, KeyModifiers>, 2>& keyMappings)
            : m_MouseClickMapping(mouseClickMapping)
            , m_KeyMappings(keyMappings)
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
    };
} // namespace nate::Modules::GUI