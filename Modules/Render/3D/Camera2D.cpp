#include "Camera2D.h"

#include <IWindow.h>
#include <KeyModifiers.hpp>
#include <KeyPressedInfo.hpp>
#include <Keys.h>
#include <LinearAlgebra/Vector3.hpp>

#include <functional>

namespace Ignosi::Modules::Render
{
    Camera2D::Camera2D(GUI::IWindow* pWindow)
        : Camera(pWindow)
        , m_PanUpMap([this]() { PanUp(); })
        , m_PanDownMap([this]() { PanDown(); })
        , m_PanLeftMap([this]() { PanLeft(); })
        , m_PanRightMap([this]() { PanRight(); })
        , m_ZoomInMap([this]() { ZoomIn(); })
        , m_ZoomOutMap([this]() { ZoomOut(); })
        , m_Pitch(0.0)
        , m_Yaw(-std::numbers::pi_v<float> / 2.0f)
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

        m_ZoomInMap.KeyMappings({
            {{GUI::Key::Z, GUI::KeyModifiers()}, {GUI::Key::None, GUI::KeyModifiers()}}
        });
        m_ZoomOutMap.KeyMappings({
            {{GUI::Key::X, GUI::KeyModifiers()}, {GUI::Key::None, GUI::KeyModifiers()}}
        });

        Near(-1.0);
        Far(1.0);
    }

    void Camera2D::PanUp(float value)
    {
        Translate({0, -value, 0});
    }

    void Camera2D::PanDown(float value)
    {
        Translate({0, value, 0});
    }

    void Camera2D::PanLeft(float value)
    {
        Translate({value, 0, 0});
    }

    void Camera2D::PanRight(float value)
    {
        Translate({-value, 0, 0});
    }

    void Camera2D::ZoomIn(float value)
    {
        Translate({0, 0, value});
    }

    void Camera2D::ZoomOut(float value)
    {
        Translate({0, 0, -value});
    }

    void Camera2D::Update(std::chrono::nanoseconds /* time */)
    {
        Window()->ExecuteWithKeyStates([this](const GUI::KeyStateMap& keyStates) { ExecuteKeyMappings(keyStates); });
    }

    void Camera2D::ExecuteKeyMappings(const GUI::KeyStateMap& keyStates)
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

        mappingUpdate(m_ZoomInMap);
        mappingUpdate(m_ZoomOutMap);
    }
} // namespace Ignosi::Modules::Render