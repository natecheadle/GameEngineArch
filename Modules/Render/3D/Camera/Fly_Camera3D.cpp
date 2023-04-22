#include "Fly_Camera3D.h"

#include "3D/Camera/Camera3D.h"
#include "IWindow.h"
#include "KeyModifiers.hpp"
#include "KeyPressedInfo.hpp"
#include "Keys.h"

#include <functional>

namespace nate::Modules::Render
{
    Fly_Camera3D::Fly_Camera3D(GUI::IWindow* pWindow)
        : Camera3D(pWindow)
        , m_PanUpMap([this]() { PanUp(); })
        , m_PanDownMap([this]() { PanDown(); })
        , m_PanLeftMap([this]() { PanLeft(); })
        , m_PanRightMap([this]() { PanRight(); })
        , m_RotatePitchMap([this]() { RotatePitch(); })
        , m_RotateYawMap([this]() { RotateYaw(); })
        , m_RotateRollMap([this]() { RotateRoll(); })
        , m_ZoomInMap([this]() { ZoomIn(); })
        , m_ZoomOutMap([this]() { ZoomOut(); })
    {
        m_PanUpMap.KeyMappings({
            {{GUI::Key::W, GUI::KeyModifiers()}, {GUI::Key::Up, GUI::KeyModifiers()}}
        });
        m_PanDownMap.KeyMappings({
            {{GUI::Key::S, GUI::KeyModifiers()}, {GUI::Key::Down, GUI::KeyModifiers()}}
        });
        m_PanLeftMap.KeyMappings({
            {{GUI::Key::A, GUI::KeyModifiers()}, {GUI::Key::Left, GUI::KeyModifiers()}}
        });
        m_PanRightMap.KeyMappings({
            {{GUI::Key::D, GUI::KeyModifiers()}, {GUI::Key::Right, GUI::KeyModifiers()}}
        });

        m_RotatePitchMap.KeyMappings({
            {{GUI::Key::Q, GUI::KeyModifiers()}, {GUI::Key::None, GUI::KeyModifiers()}}
        });
        m_RotateYawMap.KeyMappings({
            {{GUI::Key::E, GUI::KeyModifiers()}, {GUI::Key::None, GUI::KeyModifiers()}}
        });

        m_ZoomInMap.KeyMappings({
            {{GUI::Key::Z, GUI::KeyModifiers()}, {GUI::Key::None, GUI::KeyModifiers()}}
        });
        m_ZoomOutMap.KeyMappings({
            {{GUI::Key::X, GUI::KeyModifiers()}, {GUI::Key::None, GUI::KeyModifiers()}}
        });
    }

    void Fly_Camera3D::PanUp(float value)
    {
        Translate({0, -value, 0});
    }

    void Fly_Camera3D::PanDown(float value)
    {
        Translate({0, value, 0});
    }

    void Fly_Camera3D::PanLeft(float value)
    {
        Translate({value, 0, 0});
    }

    void Fly_Camera3D::PanRight(float value)
    {
        Translate({-value, 0, 0});
    }

    void Fly_Camera3D::RotatePitch(float value)
    {
        Rotate({value, 0, 0});
    }

    void Fly_Camera3D::RotateYaw(float value)
    {
        Rotate({0, value, 0});
    }

    void Fly_Camera3D::RotateRoll(float value)
    {
        Rotate({0, 0, value});
    }

    void Fly_Camera3D::ZoomIn(float value)
    {
        Translate({0, 0, value});
    }

    void Fly_Camera3D::ZoomOut(float value)
    {
        Translate({0, 0, -value});
    }

    void Fly_Camera3D::Update(std::chrono::nanoseconds /* time */)
    {
        Window()->ExecuteWithKeyStates([this](const GUI::KeyStateMap& keyStates) { ExecuteKeyMappings(keyStates); });
    }

    void Fly_Camera3D::ExecuteKeyMappings(const GUI::KeyStateMap& keyStates)
    {
        auto mappingUpdate = [keyStates](const GUI::KeyMapping& mappings) -> void {
            for (const auto& mapping : mappings.KeyMappings())
            {
                if ((keyStates[mapping.first].first == GUI::KeyState::Pressed ||
                     keyStates[mapping.first].first == GUI::KeyState::Repeat) &&
                    keyStates[mapping.first].second == mapping.second)
                {
                    mappings.ExecuteKeyEvent();
                    return;
                }
            }
        };

        mappingUpdate(m_PanUpMap);
        mappingUpdate(m_PanDownMap);
        mappingUpdate(m_PanLeftMap);
        mappingUpdate(m_PanRightMap);

        mappingUpdate(m_RotatePitchMap);
        mappingUpdate(m_RotateYawMap);
        mappingUpdate(m_RotateRollMap);

        mappingUpdate(m_ZoomInMap);
        mappingUpdate(m_ZoomOutMap);
    }
} // namespace nate::Modules::Render
